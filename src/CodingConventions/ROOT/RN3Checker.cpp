// Author: Danilo Piparo (2015)

#include "RN3Checker.h"

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
         void RN3Checker::checkASTDecl(const clang::TypedefNameDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto declName=D->getName();
            if (declName.size() <= 2 ) { // this is shorter than _t !
               Report(D, "Typedefs and aliases names cannot be shorter than three characters.", BR);
               return ; 
            }
            if (declName.endswith("_t")) return;
            Report(D, "[sas.CodingConventions.ROOT.RN3] RN3: Typedefs and aliases names must end with a \"_t\".", BR);
         }
      }
   }
} // end namespace sas
