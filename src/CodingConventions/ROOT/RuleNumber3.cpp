// Author: Danilo Piparo (2015)

#include "RuleNumber3.h"

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
#include <llvm/ADT/StringRef.h>
#include "CheckerDisabler.h"

namespace sas
{
   namespace CodingConventions
   {
      namespace ROOT
      {
         const char* const sas::CodingConventions::ROOT::RN3Checker::checkerName = "sas.codingConventions.ROOT.RN3";

         void RN3Checker::checkASTDecl(const clang::CXXRecordDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            if (IsDisabled(D, Mgr, checkerName)) return; // Disabled by a comment
            auto firstChar = D->getName()[0];
            if ('T' == firstChar) return;
            auto Location = clang::ento::PathDiagnosticLocation(D, Mgr.getSourceManager());
            if (!BT) BT.reset(new clang::ento::BugType(this, "Variable name doesn't begin with uppercase letter", "SAS"));
            auto Report = new clang::ento::BugReport(*BT, "Class names must start with uppercase T (ROOT Coding Conventions: RN3)", Location);
            Report->setDeclWithIssue(D);
            BR.emitReport(Report);
         }
      }
   }
} // end namespace sas
