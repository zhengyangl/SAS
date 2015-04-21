// Author: Filip Bartek (2013)

#include "VarnameChecker.h"

#include <cctype>
// isupper

#include <clang/AST/ASTContext.h>
using clang::ASTContext;

#include <clang/AST/Comment.h>
using clang::comments::FullComment;
using clang::comments::BlockContentComment;
using clang::comments::VerbatimLineComment;
using clang::comments::ParagraphComment;
using clang::comments::TextComment;

#include <clang/AST/Decl.h>
using clang::VarDecl;

#include <clang/AST/DeclBase.h>
typedef clang::Decl::attr_iterator attr_iterator;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;
#ifdef USE_BUGTYPE
using clang::ento::BugReport;
#endif // USE_BUGTYPE

#ifdef USE_BUGTYPE
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
using clang::ento::BugType;
#endif // USE_BUGTYPE

#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
using clang::ento::PathDiagnosticLocation;

#include <clang/StaticAnalyzer/Core/Checker.h>
using clang::ento::Checker;
using clang::ento::check::ASTDecl;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;

#include <llvm/ADT/ArrayRef.h>
using llvm::ArrayRef;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

#include "CheckerDisabler.h"
using sas::IsDisabled;

namespace sas
{
namespace Example{
   const char* const VarnameChecker::checkerName = "sas.Example.Varname";

   void VarnameChecker::checkASTDecl(const VarDecl* D, AnalysisManager& Mgr, BugReporter& BR) const
   {
      if (IsDisabled(D, Mgr, checkerName)) return; // Disabled by a comment
      // Name of the declared variable:
      const StringRef Name = D->getName();
      const char* const NameChar = Name.data();
      // First letter of the name:
      const char FirstLetter = NameChar[0];
      // Don't emit report if FirstLetter is uppercase:
      if (isupper(FirstLetter)) return;
      Report(D, "[sas.Example.Varname] Variable name doesn't begin with an upper case letter", BR);
   }
} // end namespace Example
} // end namespace sas
