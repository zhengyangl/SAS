//== GlobalStaticChecker.h - Checks for non-const global statics --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

// Checks for global non-const statics which might not be thread-safe

#ifndef Utilities_StaticAnalyzers_GlobalStaticChecker_h
#define Utilities_StaticAnalyzers_GlobalStaticChecker_h

#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>

#include "SasException.h"
#include "SasChecker.h"

namespace sas
{
   namespace ThreadSafety{
      class GlobalStaticTraits : public CommonCheckerTraits{
      public:
         static constexpr const char* Name="sas.ThreadSafety.GlobalStatic";
         static constexpr const char* Description="Global non-const statics which might not be thread-safe";
      };

      class GlobalStaticChecker :
         public SasChecker<GlobalStaticTraits, clang::ento::check::ASTDecl<clang::VarDecl>>{
      public:
         void checkASTDecl(const clang::VarDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const;
      private:
         SasException m_exception;
      };
   }
}

#endif
