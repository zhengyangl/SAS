// Author: Aditya Kale (2015)

#include "RN9Checker.h"

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
         void RN9Checker::checkASTDecl(const clang::FieldDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto name = D->getName();
            if (2 >= name.size()) return;
            auto firstChar = name[0];
            auto secondChar = name[1];
            if ( firstChar == 'f' && std::isupper(secondChar) ) return;
            Report(D, "[sas.CodingConventions.ROOT.RN9] RN9Fields: Ill formed field name. The first letter must be f and the second a capital letter.", BR);
         }
         void RN9Checker::checkASTDecl(const clang::FunctionDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
         {
            auto name = D->getName();
            if (0 == name.size()) return;
            if (name == "main") return;
            auto firstChar = name[0];
            if ( std::isupper(firstChar) ) return;
            Report(D, "[sas.CodingConventions.ROOT.RN9] RN9Functions: Ill formed function name. The first letter must be capital letter.", BR);
         }
      }
   }
} // end namespace sas
