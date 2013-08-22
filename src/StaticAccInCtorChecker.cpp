// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

#include "StaticAccInCtorChecker.h"

#include <clang/AST/DeclBase.h>
using clang::Decl;

#include <clang/AST/DeclCXX.h>
using clang::CXXConstructorDecl;

#include <clang/AST/Stmt.h>
using clang::Stmt;

#include <clang/AST/StmtVisitor.h>
using clang::ConstStmtVisitor;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;

#include <clang/AST/Expr.h>
using clang::DeclRefExpr;

#include <clang/AST/Decl.h>
using clang::ValueDecl;

#include "dbgs.h"
using sas::dbgs;

namespace {
  class SAICVisitor : public ConstStmtVisitor<SAICVisitor> {
  private:
    BugReporter &BR;
    const Decl * const DeclWithIssue;
  public:
    SAICVisitor(const Decl * const declWithIssue, BugReporter &br)
      : BR(br), DeclWithIssue(declWithIssue) {}
    void VisitChildren(const Stmt * const S) {
      for (Stmt::const_child_iterator I = S->child_begin(), E = S->child_end();
           I != E; ++I) {
        if (const Stmt * const child = *I)
          Visit(child);
      }
    }
    void VisitStmt(const Stmt * const S) { VisitChildren(S); }
    void VisitDeclRefExpr(const DeclRefExpr * const DRE);
    //void VisitDeclStmt(DeclStmt *DS);
  private:
    //void VisitVarDecl(VarDecl *VD);
  };
} // end anonymous namespace

void SAICVisitor::VisitDeclRefExpr(const DeclRefExpr * const DRE)
{
  dbgs() << "0";
  if (!DRE)
    return; // Invalid expression
  const ValueDecl * const valueDecl = DRE->getDecl();
  dbgs() << "1";
  if (!valueDecl)
    return;
  dbgs() << "3 ";
  //dbgs() << valueDecl->getName() << "\n";
  valueDecl->printName(dbgs());
  dbgs() << "\n";
  //dbgs() << DRE->getFoundDecl()->getName() << "\n";
  // Check DRE. Does it reference a static variable?
  //const ValueDecl * const valueDecl = DRE->getDecl();
  return;
}

namespace sas {
  const char * const StaticAccInCtorChecker::checkerName =
    "security.StaticAccInCtor";

  void StaticAccInCtorChecker::checkASTDecl(const CXXConstructorDecl *D,
                                            AnalysisManager &Mgr,
                                            BugReporter &BR) const
  {
    if (!D)
      return; // Invalid declaration
    if (!D->isThisDeclarationADefinition())
      return; // No definition here
    assert(D->hasBody());
    const Stmt * const body = D->getBody();
    SAICVisitor walker(D, BR);
    walker.Visit(body);
    //D->dump();
    //dbgs() << ".";
    // Parse through the statements. Look for access to static data.
    // Also parse the initializers.
  }

  void StaticAccInCtorChecker::checkASTCodeBody(const Decl *D,
                                                AnalysisManager& mgr,
                                                BugReporter &BR) const
  {
    if (!D)
      return; // invalid D
    //D->dump();
  }
} // end namespace sas
