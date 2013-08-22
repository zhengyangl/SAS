// -*-c++-*-

// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

// Clang Static Analyzer checker
// Reports a bug when a static global variable is accessed from constructor.

#ifndef SAS_CHECKERS_STATICACCINCTORCHECKER_H
#define SAS_CHECKERS_STATICACCINCTORCHECKER_H

// If USE_BUGTYPE is defined, reporting is done using the method
// `BugReporter.emitReport`. Otherwise, reporting uses the method
// `BugReporter.EmitBasicReport` (which in turn calls `BugReporter.emitReport`).
// The first method may be faster as it only defines `BugType` object once.
#define USE_BUGTYPE

#include <clang/StaticAnalyzer/Core/Checker.h>
// clang::ento::Checker
// clang::CXXConstructorDecl
// clang::ento::AnalysisManager
// clang::ento::BugReporter

#ifdef USE_BUGTYPE
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
#endif // USE_BUGTYPE

namespace sas {
  class StaticAccInCtorChecker : public clang::ento::Checker<
    clang::ento::check::ASTDecl<clang::CXXConstructorDecl>,
    clang::ento::check::ASTCodeBody > {
  public:
    void checkASTDecl(const clang::CXXConstructorDecl *D,
                      clang::ento::AnalysisManager &Mgr,
                      clang::ento::BugReporter &BR) const;
    void checkASTCodeBody(const clang::Decl *D,
                          clang::ento::AnalysisManager& mgr,
                          clang::ento::BugReporter &BR) const;

#ifdef USE_BUGTYPE
  private:
    mutable clang::OwningPtr<clang::ento::BugType> BT;
#endif // USE_BUGTYPE
  private:
    static const char * const checkerName;
  };
} // end namespace sas

#endif
