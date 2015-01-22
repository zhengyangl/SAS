//== MutableMemberChecker.h - Checks for mutable members --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

// Checks for members with the mutable keyword which might not be thread-safe

#ifndef Utilities_StaticAnalyzers_MutableMemberChecker_h
#define Utilities_StaticAnalyzers_MutableMemberChecker_h

#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>

#include "SasException.h"

namespace sas
{
   class MutableMemberChecker : public clang::ento::Checker<clang::ento::check::ASTDecl<clang::FieldDecl>>
   {
      mutable std::unique_ptr<clang::ento::BuiltinBug> BT;

    public:
      void checkASTDecl(const clang::FieldDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const;

    private:
      SasException m_exception;
   };
}

#endif
