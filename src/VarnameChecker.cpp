// Filip Bartek <filip.bartek@cern.ch>

// Bases:
// sas/src/ConstCastChecker.cpp
// clang/lib/StaticAnalyzer/Checkers/CheckerDocumentation.cpp
// clang/lib/StaticAnalyzer/Checkers/IdempotentOperationChecker.cpp

#include "VarnameChecker.h"

#include <clang/StaticAnalyzer/Core/Checker.h>
using clang::ento::Checker;
using clang::ento::check::ASTDecl;

#include <clang/AST/Decl.h>
using clang::VarDecl;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;
#ifdef USE_BUGTYPE
using clang::ento::BugReport;
#endif // USE_BUGTYPE

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;

#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
using clang::ento::PathDiagnosticLocation;

#ifdef USE_BUGTYPE
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
using clang::ento::BugType;
#endif // USE_BUGTYPE

#include <cctype>
// isupper

namespace sas {
  void VarnameChecker::checkASTDecl(const VarDecl *D,
				    AnalysisManager &Mgr,
				    BugReporter &BR) const {
    // Name of the declared variable:
    const StringRef Name = D->getName();
    const char * const NameChar = Name.data();\
    // First letter of the name:
    const char FirstLetter = NameChar[0];
    // Don't emit report if FirstLetter is uppercase:
    if (isupper(FirstLetter)) return;
    // Location of the bug:
    const PathDiagnosticLocation Location =
      PathDiagnosticLocation(D, Mgr.getSourceManager());
    // Emit report:
#ifdef USE_BUGTYPE
    if (!BT)
      BT.reset(new BugType("Variable name doesn't begin with uppercase letter",
			   "SAS"));
    BugReport * Report = new BugReport(*BT,
      "Variable name doesn't begin with an uppercase letter "
      "(example variable name checker)",
      Location);
    Report->setDeclWithIssue(D);
    BR.emitReport(Report);
#else
    BR.EmitBasicReport(D,
		       "Variable name doesn't begin with uppercase letter",
		       "SAS",
		       "Variable name doesn't begin with an uppercase letter "
		         "(example variable name checker)",
		       Location);
#endif // USE_BUGTYPE
  }
} // end namespace sas
