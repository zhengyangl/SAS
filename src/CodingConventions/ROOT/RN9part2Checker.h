// Author: Aditya Kale (2015)

// Checks whether the names of functions starts with capital letter

#ifndef SAS_CHECKERS_ROOTCCRN9part2_H
#define SAS_CHECKERS_ROOTCCRN9part2_H

#include "CodingConventionChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN9part2Traits : public CommonCCTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN9part2";
      static constexpr const char* Description="Function name must begin with capital letter.";
    };

    class RN9part2Checker : public CodingConventionChecker<RN9part2Traits, clang::ento::check::ASTDecl<clang::FunctionDecl>>
    {
    public:
      void checkASTDecl(const clang::FunctionDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
