//== MutableMemberChecker.cpp - Checks for mutable members --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//
//===----------------------------------------------------------------------===//

#include "MutableMemberChecker.h"

namespace sas
{
   namespace ThreadSafety{
      void MutableMemberChecker::checkASTDecl(const clang::FieldDecl* D, clang::ento::AnalysisManager& Mgr, clang::ento::BugReporter& BR) const
      {
         if (D->isMutable() && D->getDeclContext()->isRecord()) {

            std::string buf;
            llvm::raw_string_ostream os(buf);
            os << "[sas.ThreadSafety.MutableMemberChecker] Mutable member'" << *D << "' in class, might be thread-unsafe when accessing via a const handle.";

            Report(D, os.str().c_str(), BR);
            return;
         }
      }
   }
}
