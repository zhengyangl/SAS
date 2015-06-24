/*
  A black/white List checker disabler
*/

#ifndef BLACK_WHITE_LIST_CHECKER_DISABLER_H
#define BLACK_WHITE_LIST_CHECKER_DISABLER_H

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
#include <fnmatch.h>
#include "llvm/Support/YAMLTraits.h"
#include <regex>

class BlackWhiteListCheckerDisabler {
   bool PathWildCardMatch(std::vector<std::string> &, const std::string &) const;
   bool RegexMatch(std::vector<std::string> &, const std::string &) const;
   bool PrefixMatch(std::vector<std::string> &, const std::string &) const;
   bool ParseBlackWhiteField(std::string &) const;
   bool CheckBlackWhiteListConfiguration() const;
   void WalkDeclContext(const clang::DeclContext *);

   const clang::SourceLocation sLoc;
   std::string checker;
   std::string struct_name;
   std::string class_name;
   std::string namespace_name;
   llvm::StringRef relPath;

public:
   BlackWhiteListCheckerDisabler(const clang::Decl *,
                                 const char *,
                                 clang::ento::BugReporter &,
                                 const clang::SourceLocation &);

   BlackWhiteListCheckerDisabler(clang::ento::CheckerContext &C,
                                 const char *checker,
                                 clang::ento::BugReporter &BR,
                                 const clang::SourceLocation &sLoc);
   bool isDisabled();

   struct blackWhiteItem {
      std::string blackOrWhite = "B";
      std::vector<std::string> checkerList, namespaceList, filePathList, classList, structList;
   };

   struct configuration {
      std::vector<blackWhiteItem> blackWhiteSequence;
   };
};


LLVM_YAML_IS_SEQUENCE_VECTOR(std::string)
LLVM_YAML_IS_SEQUENCE_VECTOR(BlackWhiteListCheckerDisabler::blackWhiteItem)
namespace llvm{
   namespace yaml{
      template <> struct MappingTraits<BlackWhiteListCheckerDisabler::blackWhiteItem> {
         static void mapping(IO &io, BlackWhiteListCheckerDisabler::blackWhiteItem &info) {
            io.mapRequired("B/W", info.blackOrWhite);
            io.mapOptional("CHECKER", info.checkerList);
            io.mapOptional("NAMESPACE", info.namespaceList);
            io.mapOptional("CLASS", info.classList);
            io.mapOptional("STRUCT", info.structList);
            io.mapOptional("FILE_PATH", info.filePathList);
         }
      };

      template <> struct MappingTraits<BlackWhiteListCheckerDisabler::configuration> {
         static void mapping(IO &io, BlackWhiteListCheckerDisabler::configuration &info) {
            io.mapOptional("CONFIGURATION", info.blackWhiteSequence);
         }
      };
   }
}

#endif
