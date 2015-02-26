//==-- UsingNamespace.h - Checks for using namespace and using std:: in headers --------------*- C++ -*--==//
//
// by Shahzad Malik MUZAFFAR [ Shahzad.Malik.MUZAFFAR@cern.ch ]
//    Filip Bartek (2013)
//    Danilo Pipar (2015)
//
//===----------------------------------------------------------------------===//

// Checks for 'using namespace' or 'using std::' in header files

#ifndef Utilities_StaticAnalyzers_UsingNamespace_h
#define Utilities_StaticAnalyzers_UsingNamespace_h

#include "SasChecker.h"

namespace sas {
   namespace CodingConventions {
      namespace General {
         class NoUsingNamespaceInHeadersTraits : public CommonCheckerTraits {
         public:
            static constexpr const char *Name = "sas.CodingConventions.General.NoUsingNamespaceInHeaders";
            static constexpr const char *Description = "Do not use \"using namespace\" or \"using std::\" in headers.";
         };
         class NoUsingNamespaceInHeaders : public SasChecker < NoUsingNamespaceInHeadersTraits,
            clang::ento::check::ASTDecl<clang::UsingDirectiveDecl>,
               clang::ento::check::ASTDecl<clang::UsingDecl >> {
         public:
            void checkASTDecl(const clang::UsingDirectiveDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const;
            void checkASTDecl(const clang::UsingDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const;

         private:
            bool isDeclOK(const clang::NamedDecl *D, clang::ento::BugReporter &BR) const;
         };
      }
   }
}
#endif
