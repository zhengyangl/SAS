// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

// Inspiration:
// LLVMConventionsChecker (StmtVisitor)

#include "StaticAccInCtorChecker.h"

#include <clang/Analysis/AnalysisContext.h>
using clang::AnalysisDeclContext;

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

#include <llvm/ADT/OwningPtr.h>
using llvm::OwningPtr;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

#include "dbgs.h"
using sas::dbgs;

namespace {
  class SAICVisitor : public ConstStmtVisitor<SAICVisitor> {
  private:
    BugReporter &BR;
    const Decl * const DeclWithIssue;
    OwningPtr<BugType>& BT;
    AnalysisManager& Mgr;
  public:
    SAICVisitor(const Decl * const declWithIssue,
                BugReporter &br,
                OwningPtr<BugType>& BT,
                AnalysisManager& Mgr);
    void VisitChildren(const Stmt * const S);
    void VisitStmt(const Stmt * const S);
    void VisitDeclRefExpr(const DeclRefExpr * const DRE);
  };

  void EmitReport(const DeclRefExpr * const DRE,
                  const VarDecl * const varDecl,
                  const Decl * const DeclWithIssue,
                  BugReporter& BR,
                  OwningPtr<BugType>& BT,
                  AnalysisManager& Mgr);
} // end anonymous namespace

SAICVisitor::SAICVisitor(const Decl * const declWithIssue,
                         BugReporter &br,
                         OwningPtr<BugType>& BT,
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
  if (!DRE)
    return; /// Invalid reference expression
  /// `DRE` is a reference to a declared variable, function, enum etc.
  const ValueDecl * const valueDecl = DRE->getDecl();
  assert(valueDecl);
  if (!valueDecl)
    return; /// Invalid value declaration
  /// `valueDecl` is the declaration referred by `DRE`.
  const VarDecl * const varDecl = dyn_cast<VarDecl>(valueDecl);
  if (!varDecl)
    return; /// Not a variable declaration
  /// `varDecl` (that is `valueDecl`) is a variable delcaration.
  /// `DRE` refers to a variable.
  const StorageClass storageClass = varDecl->getStorageClass();
  if (!storageClass)
    return; /// No storage class
  /// `storageClass` is the storage class of `varDecl`.
  /// `DRE` refers to a variable with a storage class.
  if (storageClass != clang::SC_Static)
    return; /// Non-static storage class
  /// `storageClass` is static.
  /// `DRE` refers to a static variable.
  /// Bingo!
  EmitReport(DRE, varDecl, DeclWithIssue, BR, BT, Mgr);
}


namespace {
  void
  EmitReport(const DeclRefExpr * const DRE,
             const VarDecl * const varDecl,
             const Decl * const DeclWithIssue,
             BugReporter& BR,
             OwningPtr<BugType>& BT,
             AnalysisManager& Mgr)
  {
    static const char * const desc = "Static variable accessed in constructor";
    if (!BT)
      BT.reset(new BugType("Static variable accessed in constructor",
                           "Security"));
    const SourceManager& SM = BR.getSourceManager();
    AnalysisDeclContext * const analysisDeclContext =
      Mgr.getAnalysisDeclContext(DeclWithIssue);
    const LocationOrAnalysisDeclContext lac = analysisDeclContext;
    const PathDiagnosticLocation l =
      PathDiagnosticLocation::createBegin(DRE, SM, lac);
    BugReport * const report = new BugReport(*BT, desc, l);
    report->setDeclWithIssue(DeclWithIssue);
    report->addRange(DRE->getSourceRange());
    BR.emitReport(report);
    return;
  }
} // anonymous namespace

const char * const
sas::StaticAccInCtorChecker::checkerName = "security.StaticAccInCtor";

void
sas::StaticAccInCtorChecker::checkASTDecl(const CXXConstructorDecl *D,
                                          AnalysisManager &Mgr,
                                          BugReporter &BR) const
{
  if (!D)
    return; /// Invalid declaration
  if (!D->isThisDeclarationADefinition())
    return; /// No definition attached to declaration
  /// `D` has a definition attached.
  assert(D->hasBody());
  const Stmt * const body = D->getBody();
  assert(body);
  /// `body` is the definition of `D`.
  SAICVisitor walker(D, BR, BT, Mgr);
  walker.Visit(body);
  // TODO: Parse the initializers.
}
