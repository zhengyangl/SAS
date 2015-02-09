// Author: Danilo Piparo (2015)

#include "RN11Checker.h"

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
         void RN11Checker::checkASTDecl(const clang::VarDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            bool isMemberStatic = D->isStaticDataMember() &&
                                  !D->isStaticLocal(); 
            if (!isMemberStatic) return;
            auto varName = D->getName();
            if (varName.size() > 1 && varName.startswith("fg")) return;
            Report(D, "RN11: The names of static data members must begin with \"fg\".", BR);
         }
      }
   }
} // end namespace sas
