// Author: Danilo Piparo (2015)

#include "RN13Checker.h"

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
#include <llvm/ADT/StringRef.h>

namespace sas
{
   namespace CodingConventions
   {
      namespace ROOT
      {
         void RN13Checker::checkASTDecl(const clang::EnumConstantDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto declName=D->getName();
            if (declName.size()>2 && declName[0]=='k' && std::isupper(declName[1])) return;
            Report(D, "RN6: Enumeration constants names must start with \"k\" and the second letter must be uppercase.", BR);
         }
      }
   }
} // end namespace sas
