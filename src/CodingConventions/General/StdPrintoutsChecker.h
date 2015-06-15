//==-- StdPrintoutsChecker.h - Checks for debugging cout,cerr,printf printouts -- C++ -*--==//
//
// by Zhengyang Liu [ zhengyangl@hotmail.com ]
//
//===------------------------------------------------------------------------------------===//

#include "SasChecker.h"

#ifndef STD_PRINTOUTS_CHECKER_H
#define STD_PRINTOUTS_CHECKER_H

#include "SasChecker.h"

namespace sas {
   namespace CodingConventions {
      namespace General {
         class StdPrintoutsTraits : public CommonCheckerTraits {
         public:
            static constexpr const char *Name = "sas.CodingConventions.General.StdPrintouts";
            static constexpr const char *Description = "Big software systems always come with an advanced logging infrastructure. The usage of printouts may be considered an issue.";
         };

         class StdPrintoutsChecker :
            public SasChecker <StdPrintoutsTraits,
                               clang::ento::check::PreStmt<clang::CallExpr>>
         {
         public:
            void checkPreStmt(const clang::CallExpr *,
                              clang::ento::CheckerContext &) const;
         };
      }
   }
}
#endif
