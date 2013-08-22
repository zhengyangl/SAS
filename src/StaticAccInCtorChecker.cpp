// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

#include "StaticAccInCtorChecker.h"

using clang::CXXConstructorDecl;
using clang::ento::AnalysisManager;
using clang::ento::BugReporter;
using clang::Decl;

namespace sas {
  const char * const StaticAccInCtorChecker::checkerName =
    "security.StaticAccInCtor";

  void StaticAccInCtorChecker::checkASTDecl(const CXXConstructorDecl *D,
                                            AnalysisManager &Mgr,
                                            BugReporter &BR) const
  {
  }

  void StaticAccInCtorChecker::checkASTCodeBody(const Decl *D,
                                                AnalysisManager& mgr,
                                                BugReporter &BR) const
  {
  }
} // end namespace sas
