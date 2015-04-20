//== ConstCastAwayChecker.cpp - Checks for removed const qualfiers --------------*- C++ -*--==//
//
// Check in a generic way if an explicit cast removes a const qualifier.
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//    Filip Bartek [ Filip.Bartek@cern.ch ] (2013)
//
//===----------------------------------------------------------------------===//

#include "ConstCastAwayChecker.h"
#include "SasSupport.h"

namespace sas
{
   namespace ThreadSafety
   {
      void ConstCastAwayChecker::checkPreStmt(const clang::ExplicitCastExpr* CE, clang::ento::CheckerContext& C) const
      {
         const clang::Expr* E = CE->getSubExpr();
         clang::ASTContext& Ctx = C.getASTContext();
         clang::QualType OrigTy = Ctx.getCanonicalType(E->getType());
         clang::QualType ToTy = Ctx.getCanonicalType(CE->getType());

         if (support::isConst(OrigTy) && !support::isConst(ToTy)) {
            clang::ento::ExplodedNode* errorNode = C.addTransition();
            if (errorNode) {
               const char* msg = "[sas.ThreadSafety.ConstCastAwayChecker]const qualifier was removed via a cast, this may result in thread-unsafe code.";
               Report(CE, msg, C);
            }
         }
      }
   }
}
