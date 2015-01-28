// -*-c++-*-

// Author: Danilo Piparo (2015)

// Checks whether the names of classes start with an uppercase T

#ifndef SAS_CHECKERS_ROOTCCRN3_H
#define SAS_CHECKERS_ROOTCCRN3_H

#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/AST/DeclCXX.h>

namespace sas
{
   namespace CodingConventions
   {
      namespace ROOT
      {
         class RN3Checker : public clang::ento::Checker<clang::ento::check::ASTDecl<clang::CXXRecordDecl>>
         {
          public:
            void checkASTDecl(const clang::CXXRecordDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const;
          private:
            mutable std::unique_ptr<clang::ento::BugType> BT;
            static const char * const checkerName;
         };
      }
   }
} // end namespace sas

#endif
