//== GlobalStaticChecker.cpp - Checks for non-const global statics --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

#include "GlobalStaticChecker.h"

#include "SasSupport.h"

namespace sas
{
   namespace ThreadSafety{
      void GlobalStaticChecker::checkASTDecl(const clang::VarDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
      {
         if (D->getTSCSpec() == clang::ThreadStorageClassSpecifier::TSCS_thread_local) return;
         clang::QualType t = D->getType();
         if ((D->getStorageClass() == clang::SC_Static) && !D->isStaticDataMember() && !D->isStaticLocal() && !support::isConst(t)) {
            std::string buf;
            llvm::raw_string_ostream os(buf);
            os << "[sas.ThreadSafety.GlobalStaticChecker] Non-const variable '" << *D << "' is static and might be thread-unsafe";

            Report(D, os.str().c_str(), BR);
            return;
         }
      }
   }
}
