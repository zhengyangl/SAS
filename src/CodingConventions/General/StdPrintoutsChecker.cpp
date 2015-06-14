#include "StdPrintoutsChecker.h"
namespace sas {
   namespace CodingConventions {
      namespace General {
         void StdPrintoutsChecker::checkPreStmt(const clang::CallExpr *E,
                                                clang::ento::CheckerContext &C) const
         {
            for(clang::Stmt::const_child_iterator I = E->child_begin(), F = E->child_end();
                I != F;
                ++I)
            {
               const clang::Expr *child = llvm::dyn_cast<clang::Expr>(*I);
               if(!child) continue;
               if(llvm::isa<clang::DeclRefExpr>(child->IgnoreImpCasts())) {
                  const clang::NamedDecl *ND =
                     llvm::cast<clang::DeclRefExpr>(child->IgnoreImpCasts())->getFoundDecl();
                  std::string identifierName = ND->getNameAsString();
                  if(identifierName == "cerr" ||
                     identifierName == "cout" ||
                     identifierName == "printf")
                  {
                     const char* msg = "[sas.CodingConventions.General.StdPrintouts] Big software systems always come with an advanced logging infrastructure. The usage of printouts may be considered an issue.";
                     Report(E, msg, C);
                  }
               }

            }
         }
      } // end namespace general
   } // end namespace CodingConventions
} // end namespace sas
