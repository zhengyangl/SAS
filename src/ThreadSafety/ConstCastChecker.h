//== ConstCastChecker.h - Checks for const_cast<> --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//    Filip Bartek (2013)
//
//===----------------------------------------------------------------------===//

// Checks for casts which remove const qualifier and might result in thread-unsafe code

#ifndef Utilities_StaticAnalyzers_ConstCastChecker_h
#define Utilities_StaticAnalyzers_ConstCastChecker_h

#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>

// [FB] Added missing #include
#include <clang/AST/ExprCXX.h>

#include "SasException.h"
#include "SasChecker.h"

namespace sas
{
   namespace ThreadSafety {
      class ConstCastTraits : public CommonCheckerTraits{
      public:
         static constexpr const char* Name="sas.ThreadSafety.ConstCast";
         static constexpr const char* Description="Casts which remove const qualifer might result in thread-unsafe code";
      };
      class ConstCastChecker :
         public SasChecker<ConstCastTraits, clang::ento::check::PreStmt<clang::CXXConstCastExpr>>{
      public:
         mutable std::unique_ptr<clang::ento::BugType> BT;
         void checkPreStmt(const clang::CXXConstCastExpr* CE, clang::ento::CheckerContext& C) const;
      private:
         static const char* const checkerName;
      };
   }
}

#endif
