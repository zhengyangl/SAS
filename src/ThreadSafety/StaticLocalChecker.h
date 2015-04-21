//== StaticLocalChecker.h - Checks for non-const static locals --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

// Checks for non-const method local statics which might not be thread-safe

#ifndef Utilities_StaticAnalyzers_StaticLocalChecker_h
#define Utilities_StaticAnalyzers_StaticLocalChecker_h

#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>

#include "SasException.h"
#include "SasChecker.h"

namespace sas
{
   namespace ThreadSafety
   {
      class StaticLocalTraits : public CommonCheckerTraits{
      public:
         static constexpr const char* Name="sas.ThreadSafety.StaticLocal";
         static constexpr const char* Description="Non-const method local static which might not be thread-safe";
      };
      class StaticLocalChecker : public SasChecker<StaticLocalTraits, clang::ento::check::ASTDecl<clang::VarDecl>>{
      public:
         void checkASTDecl(const clang::VarDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const;
      };
   }
}

#endif
