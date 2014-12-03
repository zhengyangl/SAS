// -*-c++-*-

// Author: Filip Bartek (2013)

// Clang Static Analyzer checker
// Reports a bug when a static global variable is accessed from constructor
// and the variable can't be proven to be initialized by a constant expression.

#ifndef SAS_CHECKERS_GLOBALACCINCTORCHECKER_H
#define SAS_CHECKERS_GLOBALACCINCTORCHECKER_H

// Forward declarations:

namespace clang {
   namespace ento {
      class BugType;
   }
}

#include <clang/StaticAnalyzer/Core/Checker.h>
// clang::ento::Checker
// clang::CXXConstructorDecl
// clang::ento::AnalysisManager
// clang::ento::BugReporter

namespace sas {
   class GlobalAccInCtorChecker : public clang::ento::Checker <
         clang::ento::check::ASTDecl<clang::CXXConstructorDecl> > {
   public:
      void checkASTDecl(const clang::CXXConstructorDecl *D,
                        clang::ento::AnalysisManager &Mgr,
                        clang::ento::BugReporter &BR) const;
   private:
      mutable std::unique_ptr<clang::ento::BugType> BT;
   private:
      static const char *const checkerName;
   };
} // end namespace sas

#endif
