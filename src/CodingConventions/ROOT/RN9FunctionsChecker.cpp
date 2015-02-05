// Author: Aditya Kale (2015)

#include "RN9FunctionsChecker.h"

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
#include <llvm/ADT/StringRef.h>

#include <cctype>

namespace sas
{
   namespace CodingConventions
   {
      namespace ROOT
      {
         void RN9FunctionsChecker::checkASTDecl(const clang::FunctionDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto firstChar = D->getName()[0];
            if ( std::isupper(firstChar) ) return;
            Report(D, "RN9Functions: Ill formed function name. The first letter must be capital letter.", BR);
         }
      }
   }
} // end namespace sas
