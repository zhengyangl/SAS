//== ConstCastChecker.cpp - Checks for const_cast<> --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//    Filip Bartek (2013)
//
//===----------------------------------------------------------------------===//

#include "ConstCastChecker.h"

#include "CheckerDisabler.h"
using sas::IsDisabled;

namespace sas
{
   namespace ThreadSafety {
      const char* const ConstCastChecker::checkerName = "sas.threadsafety.ConstCast";

      void ConstCastChecker::checkPreStmt(const clang::CXXConstCastExpr* CE, clang::ento::CheckerContext& C) const
      {
         if (IsDisabled(CE, C, checkerName)) return; // Disabled by comment
         clang::ento::ExplodedNode* errorNode = C.addTransition();
         if (errorNode) {
            const char* msg = "const_cast was used, this may result in thread-unsafe code.";
            Report(CE, msg, C);
         }
      }
   }
}
