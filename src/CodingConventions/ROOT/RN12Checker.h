// Author: Danilo Piparo (2015)

// Checks whether local variable names start with a lowercase letter

#ifndef SAS_CHECKERS_ROOTCCRN12_H
#define SAS_CHECKERS_ROOTCCRN12_H

#include "SasChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN12Traits : public CommonCheckerTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN12";
      static constexpr const char* Description="Local variables names start with a lowercase letter.";
    };

    class RN12Checker : public SasChecker<RN12Traits, clang::ento::check::ASTDecl<clang::VarDecl>>
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
