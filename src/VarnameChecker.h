// -*-c++-*-

// Checks whether the names of the variables start with an uppercase letter.
// Example variable name checker.

// Filip Bartek <filip.bartek@cern.ch>

// LLVM style:
// (Based on llvm/tools/clang/include/clang/StaticAnalyzer/ClangCheckers.h)
#ifndef SAS_CHECKERS_VARNAMECHECKER_H
#define SAS_CHECKERS_VARNAMECHECKER_H

// CMS Static Analyzer style:
//#ifndef Utilities_StaticAnalyzers_VarnameChecker_h
//#define Utilities_StaticAnalyzers_VarnameChecker_h

// If USE_BUGTYPE is defined, reporting is done using the method
// `BugReporter.emitReport`. Otherwise, reporting uses the method
// `BugReporter.EmitBasicReport` (which in turn calls `BugReporter.emitReport`).
// The first method may be faster as it only defines `BugType` object once.
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
// clang::ento::BugType

//#include <?>
// clang::OwningPtr
#endif // USE_BUGTYPE

namespace sas {

class VarnameChecker : public clang::ento::Checker<
  clang::ento::check::ASTDecl<clang::VarDecl> > {
public:
  void checkASTDecl(const clang::VarDecl *D,
		    clang::ento::AnalysisManager &Mgr,
		    clang::ento::BugReporter &BR) const;
#ifdef USE_BUGTYPE
private:
  mutable clang::OwningPtr<clang::ento::BugType> BT;
#endif // USE_BUGTYPE
};

} // end namespace sas

#endif
