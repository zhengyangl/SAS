// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

#include "StaticAccInCtorChecker.h"

#include <clang/AST/DeclBase.h>
using clang::Decl;

#include <clang/AST/DeclCXX.h>
using clang::CXXConstructorDecl;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;

#include "dbgs.h"
using sas::dbgs;

namespace sas {
  const char * const StaticAccInCtorChecker::checkerName =
    "security.StaticAccInCtor";

  void StaticAccInCtorChecker::checkASTDecl(const CXXConstructorDecl *D,
                                            AnalysisManager &Mgr,
                                            BugReporter &BR) const
  {
    if (!D)
      return; // invalid D
    //D->dump();
    dbgs() << ".";
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
