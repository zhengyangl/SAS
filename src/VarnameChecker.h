// -*-c++-*-

// Author: Filip Bartek (2013)

// Checks whether the names of the variables start with an uppercase letter.
// Example checker.

#ifndef SAS_CHECKERS_VARNAMECHECKER_H
#define SAS_CHECKERS_VARNAMECHECKER_H

// If USE_BUGTYPE is defined, reporting is done using the method
// `BugReporter.emitReport`. Otherwise, reporting uses the method
// `BugReporter.EmitBasicReport` (which in turn calls `BugReporter.emitReport`).
// The first method may be faster as it only defines `BugType` object once.
// Both possibilities are implemented in `VarnameChecker` for demonstration
// purpose.
#define USE_BUGTYPE

#include <clang/StaticAnalyzer/Core/Checker.h>
// clang::ento::Checker
// clang::ento::check::ASTDecl

#include <clang/AST/Decl.h>
// clang::VarDecl

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
// clang::ento::AnalysisManager

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
// clang::ento::BugReporter

#ifdef USE_BUGTYPE
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#endif // USE_BUGTYPE

namespace sas {
   class VarnameChecker : public clang::ento::Checker <
         clang::ento::check::ASTDecl<clang::VarDecl> > {
   public:
      void checkASTDecl(const clang::VarDecl *D,
                        clang::ento::AnalysisManager &Mgr,
                        clang::ento::BugReporter &BR) const;
#ifdef USE_BUGTYPE
   private:
      mutable std::unique_ptr<clang::ento::BugType> BT;
#endif // USE_BUGTYPE
   private:
      static const char *const checkerName;
   };
} // end namespace sas

#endif
