// Author: Danilo Piparo (2015)

#include "RN4Checker.h"

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
         void RN4Checker::checkASTDecl(const clang::CXXRecordDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto declName=D->getName();
            if (declName.size() == 0) return ; // something fishy, we bail out.
            if (declName.size() < 3){
               Report(D, "Name is too short for a class/struct.", BR);
               return;
            }

            auto firstChar = D->getName()[0];
            auto secondChar = D->getName()[1];
            auto thirdChar = D->getName()[2];
            if ('T' == firstChar && std::isupper(secondChar) && !std::isupper(thirdChar)) return;
            Report(D, "RN4: Ill formed class/struct name. The first letter must be a \"T\", the second uppercase and the third lowercase.", BR);
         }
      }
   }
} // end namespace sas
