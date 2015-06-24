//== ConstCastChecker.cpp - Checks for const_cast<> --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//    Filip Bartek (2013)
//
//===----------------------------------------------------------------------===//

#include "ConstCastChecker.h"

namespace sas
{
   namespace ThreadSafety {
      const char* const ConstCastChecker::checkerName = "sas.threadsafety.ConstCast";

      void ConstCastChecker::checkPreStmt(const clang::CXXConstCastExpr* CE, clang::ento::CheckerContext& C) const
      {
         clang::ento::ExplodedNode* errorNode = C.addTransition();
         if (errorNode) {
            const char* msg = "[sas.ThreadSafety.ConstCastChecker] const_cast was used, this may result in thread-unsafe code.";
            Report(CE, msg, C);
         }
      }
   }
}
