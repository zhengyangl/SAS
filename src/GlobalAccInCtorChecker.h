// -*-c++-*-

// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

// Clang Static Analyzer checker
// Reports a bug when a static global variable is accessed from constructor.

#ifndef SAS_CHECKERS_GLOBALACCINCTORCHECKER_H
#define SAS_CHECKERS_GLOBALACCINCTORCHECKER_H


//#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
namespace clang {
  namespace ento {
    class BugType; // forward declaration
    // [FB] Assuming OwningPtr doesn't require a complete type as its template
    // parameter.
  }
}

#include <clang/StaticAnalyzer/Core/Checker.h>
// clang::ento::Checker
// clang::CXXConstructorDecl
// clang::ento::AnalysisManager
// clang::ento::BugReporter

#include <llvm/ADT/OwningPtr.h>
// clang::OwningPtr

namespace sas {
  class GlobalAccInCtorChecker : public clang::ento::Checker<
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
