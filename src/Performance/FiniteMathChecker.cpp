#include "FiniteMathChecker.h"

namespace sas {
   namespace Performance {
      void FiniteMathChecker::checkPreStmt(const clang::CallExpr *CE, clang::ento::CheckerContext &C) const
      {

         const clang::ento::ProgramStateRef state = C.getState();
         const clang::LocationContext *LC = C.getLocationContext();
         const clang::Expr *Callee = CE->getCallee();
         const clang::FunctionDecl *FD = state->getSVal(Callee, LC).getAsFunctionDecl();

         if (!FD) return;

         // Get the name of the callee.
         clang::IdentifierInfo *II = FD->getIdentifier();
         if (!II) // if no identifier, not a simple C function
            return;

         if (!II->isStr("isnan") && !II->isStr("isinf") &&
             !II->isStr("__isnan") && !II->isStr("__isinf") &&
             !II->isStr("__isnanf") && !II->isStr("__isinff")) return;

         clang::ento::ExplodedNode *N = C.generateSink();
         if (!N) return;

         const char* msg = "The function isnan/isinf does not work when fast-math is enabled. Please check the bits of the floating point number instead.";

         Report(CE, msg, C);
      }
   }
}
