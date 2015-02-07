// Author: Danilo Piparo (2015)

// Enumerations names start with E

#ifndef SAS_CHECKERS_ROOTCCRN6_H
#define SAS_CHECKERS_ROOTCCRN6_H

#include "SasChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN6Traits : public CommonCheckerTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN6";
      static constexpr const char* Description="Enumeration names begin with \"E\".";
    };

    class RN6Checker : public SasChecker<RN6Traits, clang::ento::check::ASTDecl<clang::EnumDecl>>
    {
    public:
      void checkASTDecl(const clang::EnumDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
