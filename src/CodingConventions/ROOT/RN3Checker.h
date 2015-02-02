// Author: Danilo Piparo (2015)

// Types end with a "_t"

#ifndef SAS_CHECKERS_ROOTCCRN3_H
#define SAS_CHECKERS_ROOTCCRN3_H

#include "CodingConventionChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN3Traits : public CommonCCTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN3";
      static constexpr const char* Description="Types' names end with a \"_t\".";
    };

    class RN3Checker : public CodingConventionChecker<RN3Traits, clang::ento::check::ASTDecl<clang::TypedefNameDecl>>
    {
    public:
      void checkASTDecl(const clang::TypedefNameDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
