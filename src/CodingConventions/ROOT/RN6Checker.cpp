// Author: Danilo Piparo (2015)

#include "RN6Checker.h"

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
         void RN6Checker::checkASTDecl(const clang::EnumDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto declName=D->getName();
            if (declName.size()<2 || 
               (declName.startswith("E") && !std::isupper(declName[1]))) return;
            Report(D, "RN6: Enumerations names must start with \"E\" and the second letter must be lowercase.", BR);
         }
      }
   }
} // end namespace sas
