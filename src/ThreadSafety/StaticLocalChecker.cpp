//== StaticLocalChecker.cpp - Checks for non-const static locals --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

#include "StaticLocalChecker.h"

#include "SasSupport.h"
#include <iostream>

namespace sas
{
   namespace ThreadSafety{
      void StaticLocalChecker::checkASTDecl(const clang::VarDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
      {
         clang::QualType t = D->getType();

         if (((D->isStaticLocal() || D->isStaticDataMember()) && D->getTSCSpec() != clang::ThreadStorageClassSpecifier::TSCS_thread_local) && !support::isConst(t)) {

            std::string buf;
            llvm::raw_string_ostream os(buf);
            os << "[sas.ThreadSafety.StaticLocalChecker] Non-const variable '" << *D << "' is local static and might be thread-unsafe";

            Report(D, os.str().c_str(), BR);
            return;
         }
      }
   }
}
