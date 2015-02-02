// Author: Danilo Piparo (2015)

// Enumerator constants names start with k

#ifndef SAS_CHECKERS_ROOTCCRN13_H
#define SAS_CHECKERS_ROOTCCRN13_H

#include "CodingConventionChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN13Traits : public CommonCCTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN13";
      static constexpr const char* Description="Enumeration constants start with \"k\".";
    };

    class RN13Checker : public CodingConventionChecker<RN13Traits, clang::ento::check::ASTDecl<clang::EnumConstantDecl>>
    {
    public:
      void checkASTDecl(const clang::EnumConstantDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
