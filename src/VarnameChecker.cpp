// Filip Bartek <filip.bartek@cern.ch>

// Bases:
// sas/src/ConstCastChecker.cpp
// clang/lib/StaticAnalyzer/Checkers/CheckerDocumentation.cpp
// clang/lib/StaticAnalyzer/Checkers/IdempotentOperationChecker.cpp

#include "VarnameChecker.h"

#include <clang/StaticAnalyzer/Core/Checker.h>
using clang::ento::Checker;
using clang::ento::check::ASTDecl;

#include <clang/AST/Decl.h>
using clang::VarDecl;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;
#ifdef USE_BUGTYPE
using clang::ento::BugReport;
#endif // USE_BUGTYPE

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;

#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
using clang::ento::PathDiagnosticLocation;

#ifdef USE_BUGTYPE
#include <clang/StaticAnalyzer/Core/BugReporter/BugType.h>
using clang::ento::BugType;
#endif // USE_BUGTYPE

#include <cctype>
// isupper

#include <llvm/Support/raw_ostream.h>
using llvm::outs; // standard output

#include <clang/AST/DeclBase.h>
//using clang::Decl;
typedef clang::Decl::attr_iterator attr_iterator;

#include <clang/AST/ASTContext.h>
using clang::ASTContext;

#include <clang/AST/Comment.h>
using clang::comments::FullComment;
using clang::comments::BlockContentComment;
using clang::comments::VerbatimLineComment;
using clang::comments::ParagraphComment;
using clang::comments::TextComment;

#include <llvm/ADT/ArrayRef.h>
using llvm::ArrayRef;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

namespace sas {
  void VarnameChecker::checkASTDecl(const VarDecl *D,
				    AnalysisManager &Mgr,
				    BugReporter &BR) const {
    outs() << "VarnameChecker::checkASTDecl\n";
    outs() << "  ";
    D->print(outs());
    outs() << "\n";
    outs() << "  ";
    D->getLocation().print(outs(), Mgr.getSourceManager());
    outs() << "\n";
    outs() << "  " << D->hasAttrs() << "\n";
    const ASTContext& Context = D->getASTContext();
    FullComment * Comment = Context.getLocalCommentForDeclUncached(D);
    if (Comment) {
      ArrayRef<BlockContentComment *> BlockContent = Comment->getBlocks();
      {
        typedef ArrayRef<BlockContentComment *>::const_iterator const_iterator;
        const_iterator b = BlockContent.begin();
        const_iterator e = BlockContent.end();
        for (const_iterator i = b; i != e; ++i) {
          const ParagraphComment * const paragraphComment = dyn_cast<const ParagraphComment>(*i);
          if (paragraphComment) {
            typedef clang::comments::Comment::child_iterator child_iterator;
            child_iterator child_b = paragraphComment->child_begin();
            child_iterator child_e = paragraphComment->child_end();
            for (child_iterator child_i = child_b; child_i != child_e; ++child_i) {
              const TextComment * const textComment = dyn_cast<const TextComment>(*child_i);
              if (textComment) {
                outs() << "  " << textComment->getText() << "\n";
              }
            }
          }
        }
      }
    }

    for (attr_iterator it = D->attr_begin(); it != D->attr_end(); ++it) {
      outs() << "  attr\n";
    }
    // Name of the declared variable:
    const StringRef Name = D->getName();
    const char * const NameChar = Name.data();\
    // First letter of the name:
    const char FirstLetter = NameChar[0];
    // Don't emit report if FirstLetter is uppercase:
    if (isupper(FirstLetter)) return;
    // Location of the bug:
    const PathDiagnosticLocation Location =
      PathDiagnosticLocation(D, Mgr.getSourceManager());
    // Emit report:
#ifdef USE_BUGTYPE
    if (!BT)
      BT.reset(new BugType("Variable name doesn't begin with uppercase letter",
			   "SAS"));
    BugReport * Report = new BugReport(*BT,
				       "Variable name doesn't begin with an "
				       "uppercase letter "
				       "(example variable name checker)",
				       Location);
    Report->setDeclWithIssue(D);
    BR.emitReport(Report);
#else
    BR.EmitBasicReport(D,
		       "Variable name doesn't begin with uppercase letter",
		       "SAS",
		       "Variable name doesn't begin with an uppercase letter "
		       "(example variable name checker)",
		       Location);
#endif // USE_BUGTYPE
  }
} // end namespace sas
