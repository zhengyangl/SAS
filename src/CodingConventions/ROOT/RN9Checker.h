// Author: Aditya Kale (2015)
// Author: Danilo Piparo (2015)

// Checks whether the names of functions starts with capital letter and 
// wether fields with a lowercase f.

#ifndef SAS_CHECKERS_ROOTCCRN9_H
#define SAS_CHECKERS_ROOTCCRN9_H

#include "SasChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class RN9Traits : public CommonCheckerTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.RN9";
      static constexpr const char* Description="Function name must begin with capital letter, fields with a lowercase \"f\".";
    };

    class RN9Checker : public SasChecker<RN9Traits, 
                                         clang::ento::check::ASTDecl<clang::FunctionDecl>,
                                         clang::ento::check::ASTDecl<clang::FieldDecl>>
    {
    public:
      void checkASTDecl(const clang::FunctionDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
      void checkASTDecl(const clang::FieldDecl* D, 
                        clang::ento::AnalysisManager& Mgr, 
                        clang::ento::BugReporter& BR) const;
    };
  }
}
} // end namespace sas

#endif
