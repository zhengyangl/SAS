// -*-c++-*-

// The function determines whether a given Decl is disabled using a comment.
// The declaration is considered disabled when its associated special comment
// includes the substring: "sas::disable_checker(\"MyChecker\")" (where
// "MyChecker" is the (fully qualified) name of the checker (for example
// "sas.CodeRules.UsingNamespace").

// Filip Bartek <filip.bartek@cern.ch>

#ifndef SAS_CHECKERDISABLER_H
#define SAS_CHECKERDISABLER_H

namespace clang {
  class Decl; // forward declaration
  class DeclStmt; // forward declaration
}

namespace llvm {
  class StringRef; // forward declaration
}

namespace sas {
  /// Is a checker disabled on a declaration?
  /// \param decl declaration
  /// \param checkerName name of the checker
  /// \return True iff the checker checkerName is disabled on the declaration
  ///         decl
  bool IsDisabled(const clang::Decl * const decl,
                  const llvm::StringRef checkerName);
  bool IsDisabled(const clang::DeclStmt * const declStmt,
                  const llvm::StringRef checkerName);
} // end namespace sas

#endif
