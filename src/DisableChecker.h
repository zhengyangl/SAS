// -*-c++-*-

// The function determines whether a given Decl is disabled using a comment.
// The declaration is considered disabled when its associated special comment
// includes the substring: "sas::disable_checker(\"MyChecker\")" (where
// "MyChecker" is the (fully qualified) name of the checker (for example
// "sas.CodeRules.UsingNamespace").

// Filip Bartek <filip.bartek@cern.ch>

#ifndef SAS_DISABLECHECKER_H
#define SAS_DISABLECHECKER_H

//#include <clang/AST/DeclBase.h>
// clang::Decl
namespace clang {
  class Decl; // forward declaration
}

//#include <llvm/ADT/StringRef.h>
// llvm::StringRef
namespace llvm {
  class StringRef; // forward declaration
}

namespace sas {
  bool IsDisabled(const clang::Decl * const decl,
                  const llvm::StringRef checkerName);
} // end namespace sas

#endif
