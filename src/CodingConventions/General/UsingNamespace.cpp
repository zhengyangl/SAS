//==-- UsingNamespace.cpp - Checks for using namespace and using std:: in headers --------------*- C++ -*--==//
//
// by Shahzad Malik MUZAFFAR [ Shahzad.Malik.MUZAFFAR@cern.ch ]
//    Filip Bartek (2013)
//    Danilo Piparo (2015)
//
//===----------------------------------------------------------------------===//

#include "UsingNamespace.h"

namespace sas {
   namespace CodingConventions {
      namespace General {

// using namespace *
         void NoUsingNamespaceInHeaders::checkASTDecl(const clang::UsingDirectiveDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
         {
            if (isDeclOK(D, BR)) return;
            Report(D, "Detected usage of 'using namespac ' in a header.", BR);
         }

// using std::*
         void NoUsingNamespaceInHeaders::checkASTDecl(const clang::UsingDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
         {
            if (isDeclOK(D, BR)) return;
            std::string NS = D->getQualifier()->getAsNamespace()->getNameAsString();
            if (strcmp(NS.c_str(), "std") != 0) return; // Different from "std"
            Report(D, "Detected usage of 'using std::' in a header.", BR);
         }

// Declaration D is OK (function returns true) if:
// - D is not in a header file or
// - D is not in the top block (i.e. is encapsulated by a namespace or a
//   class) or
// - D is disabled by a comment in analyzed code.
// Not OK (returns false) when in a header file and top block.
// If declaration is OK, it doesn't emit a bug.
         bool NoUsingNamespaceInHeaders::isDeclOK(const clang::NamedDecl *D, clang::ento::BugReporter &BR) const
         {
            if (!D) return true;                                                                      // Invalid declaration
            if (D->getDeclContext()->getParent() != 0) return true;                                   // Not on top level in AST
            llvm::StringRef sfile = BR.getSourceManager().getPresumedLoc(D->getLocation()).getFilename(); // Get filename
            if (sfile.endswith(".h") ||
                sfile.endswith(".hpp") ||
                sfile.endswith(".H") ||
                sfile.endswith(".icc"))
            return false;
            return true;
         }

      }
   }
}
