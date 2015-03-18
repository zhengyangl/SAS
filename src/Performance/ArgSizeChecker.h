// Reports args passed by value with size>4k.

#ifndef Utilities_StaticAnalyzers_ArgSizeChecker_h
#define Utilities_StaticAnalyzers_ArgSizeChecker_h

#include "SasChecker.h"

namespace sas {
namespace Performance {
   class ArgSizeTraits : public CommonCheckerTraits{
      public:
         static constexpr const char* Name="sas.Performance.ArgSizeChecker";
         static constexpr const char* Description="Reports args passed by value with size>4k";
       };

   class ArgSizeChecker : public SasChecker<ArgSizeTraits,
                                            clang::ento::check::PreStmt<clang::CXXConstructExpr>,
                                            clang::ento::check::ASTDecl<clang::CXXMethodDecl>>
   {
    public:
      void checkPreStmt(const clang::CXXConstructExpr* ref, clang::ento::CheckerContext& C) const;
      void checkASTDecl(const clang::CXXMethodDecl* CMD, clang::ento::AnalysisManager& mgr, clang::ento::BugReporter& BR) const;
   };
}
}

#endif
