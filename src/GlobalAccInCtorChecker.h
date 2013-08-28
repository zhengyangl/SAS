// -*-c++-*-

// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

// Clang Static Analyzer checker
// Reports a bug when a static global variable is accessed from constructor.

#ifndef SAS_CHECKERS_STATICACCINCTORCHECKER_H
#define SAS_CHECKERS_STATICACCINCTORCHECKER_H

#include <clang/StaticAnalyzer/Core/Checker.h>
// clang::ento::Checker
// clang::CXXConstructorDecl
// clang::ento::AnalysisManager
// clang::ento::BugReporter

//#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
// clang::ento::BugType
namespace clang {
  namespace ento {
    class BugType; // forward declaration
    // TODO: Is this correct? Does OwningPtr require the template parameter to
    // be a complete type?
  }
}

#include <llvm/ADT/OwningPtr.h>
// clang::OwningPtr

namespace sas {
  class StaticAccInCtorChecker : public clang::ento::Checker<
    clang::ento::check::ASTDecl<clang::CXXConstructorDecl> > {
  public:
    void checkASTDecl(const clang::CXXConstructorDecl *D,
                      clang::ento::AnalysisManager &Mgr,
                      clang::ento::BugReporter &BR) const;
  private:
    mutable clang::OwningPtr<clang::ento::BugType> BT;
  private:
    static const char * const checkerName;
  };
} // end namespace sas

#endif
