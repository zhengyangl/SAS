// Author: Aditya Kale (2015)

// Checks whether the names of functions starts with capital letter

#ifndef SAS_CHECKERS_ROOTCCRN9Functions_H
#define SAS_CHECKERS_ROOTCCRN9Functions_H

#include "CodingConventionChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN9FunctionsTraits : public CommonCCTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN9Functions";
      static constexpr const char* Description="Function name must begin with capital letter.";
    };

    class RN9FunctionsChecker : public CodingConventionChecker<RN9FunctionsTraits, clang::ento::check::ASTDecl<clang::FunctionDecl>>
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
