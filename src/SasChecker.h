/*
A general interface for sas checkers
EXPERIMENTAL
*/

#ifndef SAS_CHECKER_H
#define SAS_CHECKER_H

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/DeclCXX.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <memory>
#include <regex>

// What is the meaning of these?
class CommonCheckerTraits {
public:
   static constexpr const char *BugName = "";
   static constexpr const char *BugCategory = "";
   static constexpr bool SkipMacros = true;
   static constexpr const char *PathBlackListRegex = nullptr;
};

template<class Traits, class... WHAT>
class SasChecker : public clang::ento::Checker<WHAT...> {
public:
   SasChecker(): fPathBlackListRegex(nullptr) {
      if (Traits::PathBlackListRegex) fPathBlackListRegex.reset(new std::regex(Traits::PathBlackListRegex));
   }
   // Used for registration
   static const char *GetName() {
      return Traits::Name;
   }
   static const char *GetDescription() {
      return Traits::Description;
   }
   // Used for reporting
   static const char *GetBugName() {
      return Traits::BugName;
   }
   static const char *GetBugCategory() {
      return Traits::BugCategory;
   }

private:
   std::unique_ptr<std::regex> fPathBlackListRegex;

   bool MustSkipReport(const clang::SourceLocation &sLoc, const clang::SourceManager &mgr) const {
      // Skip if invalid
      if (sLoc.isInvalid()) return true;
      // Skip if this is a macro
      if (Traits::SkipMacros && (mgr.isMacroBodyExpansion(sLoc) || mgr.isMacroArgExpansion(sLoc))) return true;
      // If this is not a user header, do not report
      auto fileCharacteristic = mgr.getFileCharacteristic(sLoc);
      if (fileCharacteristic != clang::SrcMgr::C_User) return true;
      // If the path of the file matches the blacklist regex, skip
      if (fPathBlackListRegex) {
         auto relPath = mgr.getFilename(sLoc);
         llvm::SmallString<1024> absPath = relPath;
         llvm::sys::fs::make_absolute(absPath);
         if (std::regex_match(absPath.str().str(), *fPathBlackListRegex)) return true;
      }
      // Now check if the line above this one has a comment of this form
      // "//[[SasIgnore]]"
      // Must do this properly...
      return false;
   }

public:
   void Report(const clang::Decl *D, const char *msg, clang::ento::BugReporter &BR) const {
      auto &srcMgr = BR.getSourceManager();
      auto sLoc = D->getLocation();
      if (MustSkipReport(sLoc, srcMgr)) return;
      auto DLoc = clang::ento::PathDiagnosticLocation::createBegin(D, srcMgr);
      BR.EmitBasicReport(D, this, Traits::BugName, Traits::BugCategory, msg, DLoc);
   }

   void Report(const clang::Stmt *E, const char *msg, clang::ento::CheckerContext &C) const {
      auto &srcMgr = C.getSourceManager();
      auto sLoc = E->getLocStart();
      if (MustSkipReport(sLoc, srcMgr)) return;
      if (auto errorNode = C.addTransition()) {
         auto bt = new clang::ento::BugType(this, Traits::BugName, Traits::BugCategory);
         auto br = new clang::ento::BugReport(*bt, msg, errorNode);
         br->addRange(E->getSourceRange());
         C.emitReport(br);
      }
   }
};
#endif

