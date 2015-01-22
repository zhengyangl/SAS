//== SasException.cpp -                             --------------*- C++ -*--==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ] and Patrick Gartung
//    Filip Bartek (2013)
//
//===----------------------------------------------------------------------===//

#include "SasException.h"

namespace sas
{

   bool SasException::reportGeneral(clang::ento::PathDiagnosticLocation const& path, clang::ento::BugReporter& BR) const
   {
      clang::SourceLocation SL = path.asLocation();
      if (SL.isMacroID()) {
         return false;
      }

      const clang::SourceManager& SM = BR.getSourceManager();
      clang::PresumedLoc PL = SM.getPresumedLoc(SL);

      llvm::StringRef FN = llvm::StringRef((PL.getFilename()));
      size_t found = 0;
      found += FN.count("xr.cc");
      found += FN.count("xi.cc");
      found += FN.count("LinkDef.cc");
      found += FN.count("/external/");
      found += FN.count("/lcg/");
      found += FN.count("/test/");
      found += FN.count("/sas/");
      //   found += FN.count("/FWCore/");
      if (found != 0) {
         return false;
      }

      if (SM.isInSystemHeader(SL) || SM.isInExternCSystemHeader(SL)) {
         return false;
      }
      return true;
   }

   bool SasException::reportConstCast(const clang::ento::BugReport& R, clang::ento::CheckerContext& C) const
   {
      clang::ento::BugReporter& BR = C.getBugReporter();
      const clang::SourceManager& SM = BR.getSourceManager();
      clang::ento::PathDiagnosticLocation const& path = R.getLocation(SM);
      return reportGeneral(path, BR);
   }

   bool SasException::reportConstCastAway(const clang::ento::BugReport& R, clang::ento::CheckerContext& C) const
   {
      clang::ento::BugReporter& BR = C.getBugReporter();
      const clang::SourceManager& SM = BR.getSourceManager();
      clang::ento::PathDiagnosticLocation const& path = R.getLocation(SM);
      return reportGeneral(path, BR);
   }

   bool SasException::reportGlobalStatic(clang::QualType const& t, clang::ento::PathDiagnosticLocation const& path, clang::ento::BugReporter& BR) const { return reportGeneral(path, BR); }

   bool SasException::reportMutableMember(clang::QualType const& t, clang::ento::PathDiagnosticLocation const& path, clang::ento::BugReporter& BR) const { return reportGeneral(path, BR); }

   bool SasException::reportClass(clang::ento::PathDiagnosticLocation const& path, clang::ento::BugReporter& BR) const { return reportGeneral(path, BR); }

   bool SasException::reportGlobalStaticForType(clang::QualType const& t, clang::ento::PathDiagnosticLocation const& path, clang::ento::BugReporter& BR) const { return reportGeneral(path, BR); }
}
