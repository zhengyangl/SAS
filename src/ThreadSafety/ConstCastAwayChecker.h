//== ConstCastAwayChecker.h - Checks for removed const qualfiers --------------*- C++ -*--==//
//
// Check in a generic way if an explicit cast removes a const qualifier.
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

// Checks for casts which remove const qualifier and might result in thread-unsafe code

#ifndef Utilities_StaticAnalyzers_ConstCastAwayChecker_h
#define Utilities_StaticAnalyzers_ConstCastAwayChecker_h

#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include "SasException.h"
#include "SasChecker.h"

namespace sas {
   namespace ThreadSafety {
      class ConstCastAwayTraits : public CommonCheckerTraits{
      public:
         static constexpr const char* Name="sas.ThreadSafety.ConstCastAway";
         static constexpr const char* Description="Casts which remove const qualifer might result in thread-unsafe code";
      };
      class ConstCastAwayChecker :
         public SasChecker<ConstCastAwayTraits, clang::ento::check::PreStmt<clang::ExplicitCastExpr>>{
      public:
         mutable std::unique_ptr<clang::ento::BugType> BT;
         void checkPreStmt(const clang::ExplicitCastExpr* CE, clang::ento::CheckerContext& C) const;
      };
   }
}

#endif
