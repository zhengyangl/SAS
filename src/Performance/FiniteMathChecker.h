// Reports usage of isnan and isinf.

#ifndef Utilities_StaticAnalyzers_FastMathChecker_h
#define Utilities_StaticAnalyzers_FastMathChecker_h

#include "SasChecker.h"

namespace sas {
   namespace Performance {
      class FiniteMathTraits : public CommonCheckerTraits{
      public:
         static constexpr const char* Name="sas.Performance.FiniteMathChecker";
         static constexpr const char* Description="Check usage of isnan/isinf as they do not work when fast-math is enabled.";
         static constexpr bool SkipMacros = false;
       };

      class FiniteMathChecker : public SasChecker<FiniteMathTraits, clang::ento::check::PreStmt<clang::CallExpr>>
      {
         public:
         void checkPreStmt(const clang::CallExpr* ref, clang::ento::CheckerContext& C) const;
      };
   }
}

#endif
