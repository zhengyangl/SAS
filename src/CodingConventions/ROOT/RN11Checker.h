// Author: Danilo Piparo (2015)

// Checks whether global static variables begin with g

#ifndef SAS_CHECKERS_ROOTCCRN11_H
#define SAS_CHECKERS_ROOTCCRN11_H

#include "SasChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN11Traits : public CommonCheckerTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN11";
      static constexpr const char* Description="Static global variables names start with \"g\".";
    };

    class RN11Checker : public SasChecker<RN11Traits, clang::ento::check::ASTDecl<clang::VarDecl>>
    {
    public:
      void checkASTDecl(const clang::VarDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
