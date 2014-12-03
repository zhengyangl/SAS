// Author: Filip Bartek (2013)

#include "GlobalAccInCtorChecker.h"

#include <clang/Analysis/AnalysisContext.h>
using clang::AnalysisDeclContext;

#include <clang/AST/ASTContext.h>
using clang::ASTContext;

#include <clang/AST/Decl.h>
using clang::ValueDecl;
using clang::VarDecl;
using clang::StorageClass;

#include <clang/AST/DeclBase.h>
using clang::Decl;

#include <clang/AST/DeclCXX.h>
using clang::CXXConstructorDecl;

#include <clang/AST/Expr.h>
using clang::DeclRefExpr;
using clang::Expr;

#include <clang/AST/Stmt.h>
using clang::Stmt;

#include <clang/AST/StmtVisitor.h>
using clang::ConstStmtVisitor;

#include <clang/Basic/SourceManager.h>
using clang::SourceManager;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;
using clang::ento::BugReport;

#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
using clang::ento::BugType;

#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
using clang::ento::PathDiagnosticLocation;
using clang::ento::LocationOrAnalysisDeclContext;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

// #include <unique_ptr.h>
using std::unique_ptr;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

namespace {
  // SAICVisitor traverses statements in AST and emits report on each
  // occurrence of access to a global variable unless the variable is
  // known to be initialized by a constant expression.
  class SAICVisitor : public ConstStmtVisitor<SAICVisitor> {
  private:
    BugReporter &BR;
    const Decl * const DeclWithIssue;
    std::unique_ptr<BugType>& BT;
    AnalysisManager& Mgr;
  public:
    SAICVisitor(const Decl * const declWithIssue,
                BugReporter &br,
                std::unique_ptr<BugType>& BT,
                AnalysisManager& Mgr);
    void VisitChildren(const Stmt * const S);
    void VisitStmt(const Stmt * const S);
    void VisitDeclRefExpr(const DeclRefExpr * const DRE);
  };

  // If DRE refers to a global variable that is not known to be initialized
  // by a constant expression, emits a report.
  void ProcessDeclRefExpr(const DeclRefExpr * const DRE,
                          const Decl * const DeclWithIssue,
                          BugReporter& BR,
                          std::unique_ptr<BugType>& BT,
                          AnalysisManager& Mgr);

  // Is varDecl known to have a constant initializer?
  bool HasConstantInitializer(const VarDecl * const varDecl);

  // Is the initializer expr a constant initializer?
  bool IsConstantInitializer(const Expr * const expr, ASTContext& astContext);

  // Emit a report!
  void EmitReport(const DeclRefExpr * const DRE,
                  const VarDecl * const varDecl,
                  const Decl * const DeclWithIssue,
                  BugReporter& BR,
                  std::unique_ptr<BugType>& BT,
                  AnalysisManager& Mgr);
} // end anonymous namespace


const char * const
sas::GlobalAccInCtorChecker::checkerName = "sas.security.GlobalAccInCtor";

void
sas::GlobalAccInCtorChecker::checkASTDecl(const CXXConstructorDecl *D,
                                          AnalysisManager &Mgr,
                                          BugReporter &BR) const
{
  if (!D)
    return; /// Invalid declaration
  /// `D` is a declaration of a constructor.
  if (!D->isThisDeclarationADefinition())
    return; /// No definition attached to declaration
  /// `D` has a definition attached.
  assert(D->hasBody());
  const Stmt * const body = D->getBody();
  assert(body);
  /// `body` is the definition of `D`.

  SAICVisitor walker(D, BR, BT, Mgr);

  /// Parse the initializers:
  typedef CXXConstructorDecl::init_const_iterator init_const_it;
  for (init_const_it i = D->init_begin(), e = D->init_end(); i != e; ++i) {
    const Expr * const expr = (*i)->getInit();
    if (!expr) continue;
    /// Initializer is an expression.
    walker.Visit(expr);
  }

  /// Parse the body:
  walker.Visit(body);
}

namespace {
  SAICVisitor::SAICVisitor(const Decl * const declWithIssue,
                           BugReporter &br,
                           std::unique_ptr<BugType>& BT,
                           AnalysisManager& Mgr)
    : BR(br), DeclWithIssue(declWithIssue), BT(BT), Mgr(Mgr) {}

  void
  SAICVisitor::VisitChildren(const Stmt * const S)
  {
    for (Stmt::const_child_iterator I = S->child_begin(), E = S->child_end();
         I != E; ++I) {
      if (const Stmt * const child = *I)
        Visit(child);
    }
  }

  void
  SAICVisitor::VisitStmt(const Stmt * const S)
  {
    VisitChildren(S);
  }

  void
  SAICVisitor::VisitDeclRefExpr(const DeclRefExpr * const DRE)
  {
    ProcessDeclRefExpr(DRE, DeclWithIssue, BR, BT, Mgr);
  }

  void
  ProcessDeclRefExpr(const DeclRefExpr * const DRE,
                     const Decl * const DeclWithIssue,
                     BugReporter& BR,
                     std::unique_ptr<BugType>& BT,
                     AnalysisManager& Mgr)
  {
    if (!DRE)
      return; /// Invalid reference expression
    /// `DRE` is a reference to a declared variable, function, enum etc.
    const ValueDecl * const valueDecl = DRE->getDecl();
    assert(valueDecl); /// `DeclRefExpr` should always have a value declaration
    if (!valueDecl) // [FB] Just to make sure...
      return; /// Invalid value declaration
    /// `valueDecl` is the declaration referred by `DRE`.
    const VarDecl * const varDecl = dyn_cast<VarDecl>(valueDecl);
    if (!varDecl)
      return; /// Not a variable declaration
    /// `varDecl` (that is `valueDecl`) is a variable delcaration.
    /// `DRE` refers to a variable.
    if (!varDecl->hasGlobalStorage())
      return; // Isn't a global variable
    /// `varDecl` is a global variable.
    /// `DRE` refers to a global variable.
    if (HasConstantInitializer(varDecl))
      return; // The variable has constant initializer
    /// Bingo!
    EmitReport(DRE, varDecl, DeclWithIssue, BR, BT, Mgr);
  }
  
  bool
  HasConstantInitializer(const VarDecl * const varDecl)
  {
    if (varDecl->hasInit()) {
      /// An initializer for `varDecl` is known.
      const Expr * const init = varDecl->getInit();
      assert(init);
      /// `init` is the initializer of the variable declared in `varDecl`.
      ASTContext& Ctx = varDecl->getASTContext();
      if (IsConstantInitializer(init, Ctx))
        return true; /// The variable has constant initializer
    }
    return false;
  }

  bool
  IsConstantInitializer(const Expr * const expr, ASTContext& astContext)
  {
    static const bool ForRef = false;
    /// [TODO] Confirm that `false` is the correct value for `ForRef` in the
    ///        following call to `isConstantInitializer`.
    ///        `isConstantInitializer` documentation:
    ///        http://clang.llvm.org/doxygen/Expr_8cpp_source.html#l02613
    return expr->isConstantInitializer(astContext, ForRef);
  }

  void
  EmitReport(const DeclRefExpr * const DRE,
             const VarDecl * const varDecl,
             const Decl * const DeclWithIssue,
             BugReporter& BR,
             std::unique_ptr<BugType>& BT,
             AnalysisManager& Mgr)
  {
    static const char * const desc = "Global variable accessed in constructor";
    if (!BT)
      BT.reset(new BugType("Global variable accessed in constructor",
                           "Security"));
    const SourceManager& SM = BR.getSourceManager();
    AnalysisDeclContext * const analysisDeclContext =
      Mgr.getAnalysisDeclContext(DeclWithIssue);
    const LocationOrAnalysisDeclContext lac = analysisDeclContext;
    const PathDiagnosticLocation l =
      PathDiagnosticLocation::createBegin(DRE, SM, lac);
    BugReport * const report = new BugReport(*BT, desc, l);
    // [FB] Allocation without deallocation (BugReport) - the way it's done in
    //      Clang.
    report->setDeclWithIssue(DeclWithIssue);
    report->addRange(DRE->getSourceRange());
    BR.emitReport(report);
  }
} // anonymous namespace
