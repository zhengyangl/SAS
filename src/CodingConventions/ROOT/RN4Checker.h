// Author: Danilo Piparo (2015)

// Checks whether the names of classes start with an uppercase T

#ifndef SAS_CHECKERS_ROOTCCRN4_H
#define SAS_CHECKERS_ROOTCCRN4_H

#include "CodingConventionChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN4Traits : public CommonCCTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN4";
      static constexpr const char* Description="Classes names must begin with an uppercase T.";
    };

    class RN4Checker : public CodingConventionChecker<RN4Traits, clang::ento::check::ASTDecl<clang::CXXRecordDecl>>
    {
    public:
      void checkASTDecl(const clang::CXXRecordDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
