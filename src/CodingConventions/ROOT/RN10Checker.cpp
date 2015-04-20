// Author: Danilo Piparo (2015)

#include "RN10Checker.h"

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>

namespace sas
{
   namespace CodingConventions
   {
      namespace ROOT
      {
         void RN10Checker::checkASTDecl(const clang::VarDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
             bool isGlobalStatic = D->hasGlobalStorage() &&
                                   !D->isStaticDataMember() &&
                                   !D->isStaticLocal();
             if (!isGlobalStatic) return;
             auto varName = D->getName();
             if (varName.size()!=0 && varName.startswith("g")) return;
             Report(D, "[sas.CodingConventions.ROOT.RN10] RN10: The names of static global variables must begin with \"g\".", BR);
         }
      }
   }
} // end namespace sas
