// The function determines whether a given Decl is disabled using a comment.
// The declaration is considered disabled when its associated special comment
// includes the substring: "sas::disable_checker(\"MyChecker\")" (where
// "MyChecker" is the (fully qualified) name of the checker (for example
// "sas.CodeRules.UsingNamespace").

// Filip Bartek <filip.bartek@cern.ch>

// LLVM style:
// (Based on llvm/tools/clang/include/clang/StaticAnalyzer/ClangCheckers.h)
#ifndef SAS_CHECKERS_DISABLECHECKER_H
#define SAS_CHECKERS_DISABLECHECKER_H

// CMS Static Analyzer style:
//#ifndef Utilities_StaticAnalyzers_VarnameChecker_h
//#define Utilities_StaticAnalyzers_VarnameChecker_h

#include <clang/AST/DeclBase.h>
// clang::Decl

#include <llvm/ADT/StringRef.h>
// llvm::StringRef

namespace sas {
  static _Bool isDisabled(const clang::Decl * const D,
                   const llvm::StringRef& CheckerName);
} // end namespace sas

#endif
