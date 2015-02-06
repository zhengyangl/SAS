// Author: Danilo Piparo (2015)

// Check if a cast from pointer to integer which is wrong 
// in Windows64 is performed


#ifndef SAS_CHECKERS_PTRCASTWIN_H
#define SAS_CHECKERS_PTRCASTWIN_H

#include "CodingConventionChecker.h"

namespace sas {
 namespace CodingConventions{
  namespace ROOT {
    class PtrCastWinTraits : public CommonCCTraits{
    public:
      static constexpr const char* Name="sas.CodingConventions.ROOT.PtrCastWin";
      static constexpr const char* Description="Pointers should be casted to integer types which are wide enough.";
    };

    class PtrCastWinChecker : public CodingConventionChecker<PtrCastWinTraits, clang::ento::check::PreStmt<clang::CStyleCastExpr>>{
    public:
      void checkPreStmt(const clang::CStyleCastExpr* CE, clang::ento::CheckerContext& C) const;
    };
  }
 }
} // end namespace sas

#endif
