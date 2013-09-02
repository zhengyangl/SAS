// -*-c++-*-

// Author: Filip Bartek (2013)

// This header declares several variants of the function `IsDisabled`. The
// function can be used inside a checker to determine whether the checker
// is disabled (muted) on a particular `Stmt` or `Decl`.
// See declarations of the variants of `IsDisabled` below to learn its
// interface. The top three variants allow easy plugging in `Stmt` and `Decl`
// checkers (i.e. for muting a checker on a `Stmt` or a `Decl`).

// To mute a mutable checker on a line, i.e. to make `IsDisabled` return true on
// a particular line, add a comment on a line directly preceding the line in
// question (that is the line with the statement or declaration to be muted).
// The comment must start with two slashes ("//") and must contain the text:
// sas[disable_checker : "MyChecker"]
// where MyChecker is a fully qualified name of the checker to be muted.
// Example of such comment:
// sas[disable_checker : "sas.example.Varname"]

#ifndef SAS_CHECKERDISABLER_H
#define SAS_CHECKERDISABLER_H

// Forward declarations:

namespace clang {
  class Decl;
  class DeclStmt;
  class SourceManager;
  class Stmt;
  namespace ento {
    class AnalysisManager;
    class BugReporter;
    class CheckerContext;
  }
}
namespace llvm {
  class StringRef;
}

// Declarations:

namespace sas {
  // For Stmt checkers:
  bool IsDisabled(const clang::Stmt * const stmt,
                  clang::ento::CheckerContext& checkerContext,
                  const llvm::StringRef checkerName);

  // For Decl checkers:
  bool IsDisabled(const clang::Decl * const decl,
                  clang::ento::AnalysisManager& analysisMgr,
                  const llvm::StringRef checkerName);
  bool IsDisabled(const clang::Decl * const decl,
                  clang::ento::BugReporter& bugReporter,
                  const llvm::StringRef checkerName);

  // Low-level (used by Decl variants):
  bool IsDisabled(const clang::Decl * const decl,
                  const clang::SourceManager& sourceManager,
                  const llvm::StringRef checkerName);

  // Obsolete:
  bool IsDisabledBySpecial(const clang::Decl * const decl,
                  const llvm::StringRef checkerName);
  bool IsDisabledBySpecial(const clang::DeclStmt * const declStmt,
                  const llvm::StringRef checkerName);
} // end namespace sas

#endif
