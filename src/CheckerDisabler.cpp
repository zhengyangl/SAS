// Filip Bartek <filip.bartek@cern.ch>

#define CHECKERDISABLER_PARENTDECL

#include "CheckerDisabler.h"

#include <clang/AST/DeclBase.h>
using clang::Decl;

#include <clang/AST/Stmt.h>
using clang::DeclStmt;
using clang::Stmt;

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;
static const size_t& npos = StringRef::npos;

#include <clang/AST/ASTContext.h>
using clang::ASTContext;

#include <clang/AST/Comment.h>
using clang::comments::FullComment;
using clang::comments::BlockContentComment;
using clang::comments::ParagraphComment;
using clang::comments::TextComment;

#include <llvm/ADT/ArrayRef.h>
using llvm::ArrayRef;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

#include <clang/AST/DeclGroup.h>
using clang::DeclGroupRef;

#include <clang/Basic/SourceManager.h>
using clang::SourceManager;

#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
using clang::ento::CheckerContext;

#include <clang/Analysis/AnalysisContext.h>
using clang::LocationContext;

#include <clang/Basic/SourceLocation.h>
using clang::SourceLocation;
using clang::FileID;

namespace {
  bool IsCommentedWithString(const Decl * const decl,
                             const StringRef commentString);
  string FormDisablerString(const StringRef checkerName);
  bool IsDisabledByParentDecl(const Stmt * const stmt,
                              const CheckerContext& checkerContext,
                              const StringRef checkerName);
  bool IsDisabledBySpecial(const Decl * const decl,
                           const StringRef checkerName);
}

bool sas::IsDisabled(const Decl * const decl,
                     const StringRef checkerName) {
  return IsDisabledBySpecial(decl, checkerName);
}

bool sas::IsDisabled(const DeclStmt * const declStmt,
                     const StringRef checkerName) {
  if (!declStmt)
    return false; // invalid stmt
  const DeclGroupRef declGroupRef = declStmt->getDeclGroup();
  typedef DeclGroupRef::const_iterator DeclIterator;
  DeclIterator b = declGroupRef.begin();
  DeclIterator e = declGroupRef.end();
  for (DeclIterator i = b; i != e; ++i) {
    if (IsDisabled(*i, checkerName))
      return true; // disabled decl
  }
  return false;
}

bool sas::IsDisabled(const Stmt * const stmt,
                     CheckerContext& checkerContext,
                     const StringRef checkerName)
{
  if (!stmt)
    return false; // Invalid stmt

#ifdef CHECKERDISABLER_PARENTDECL
  if (IsDisabledByParentDecl(stmt, checkerContext, checkerName))
    return true; // Parent declaration disabled
#endif // CHECKERDISABLER_PARENTDECL

  const SourceManager& sourceManager = checkerContext.getSourceManager();
  const SourceLocation stmtLoc = stmt->getLocStart();
  const unsigned stmtLineNum = sourceManager.getSpellingLineNumber(stmtLoc);
  if (stmtLineNum < 2) // FIXME: Uses 2 preceding lines instead of 1.
    return false; // Not enough preceding lines
  const FileID fileID = sourceManager.getFileID(stmtLoc);
  // Warning: Line numbers are shifted by `-1` on input to `translateLineCol`
  // or `getCharacterData` (see further in this file).
  // [FB] Reason: unknown. (Clang bug?)
  // TODO: Investigate.
  const unsigned beginLine = stmtLineNum - 2;
  // FIXME: Uses 2 preceding lines instead of just one.
  // [FB] hasn't found a way to use just one.
  const unsigned beginCol = 0;
  const unsigned endLine = stmtLineNum - 1;
  const unsigned endCol = 0;
  // Warning: Column argument of `translateLineCol` has no effect on resulting
  // `char *` pointers (`begin`, `end`).
  const SourceLocation locBegin =
    sourceManager.translateLineCol(fileID, beginLine, beginCol);
  const SourceLocation locEnd =
    sourceManager.translateLineCol(fileID, endLine, endCol);
  const char * begin = sourceManager.getCharacterData(locBegin) + 1;
    // `+ 1` gets rid of the '\n' in the beginning of the string
  const char * end = sourceManager.getCharacterData(locEnd);
  assert(end >= begin);
  const string lineString = string(begin, end - begin);
  const StringRef lineStringRef = StringRef(lineString);
  const size_t commentCol = lineStringRef.find("//");
  if (commentCol == npos)
    return false; // No `//` comment on this line
  const StringRef commentContent = lineStringRef.substr(commentCol + 2);
  const string disablerString = FormDisablerString(checkerName);
  const size_t disablerCol = commentContent.find(disablerString);
  if (disablerCol != npos)
    return true; // Disabler in comment
  return false;
}

namespace {
  string FormDisablerString(const StringRef checkerName)
  {
    ostringstream commentOss;
    commentOss << "sas[disable_checker : \"";
    commentOss << checkerName.data();
    commentOss << "\"]";
    return commentOss.str();
  }

  bool IsDisabledByParentDecl(const Stmt * const stmt,
                              const CheckerContext& checkerContext,
                              const StringRef checkerName)
  {
    const LocationContext * const locationContext =
      checkerContext.getLocationContext();
    const Decl * const decl = locationContext->getDecl();
    if (IsDisabledBySpecial(decl, checkerName))
      return true; // Parent declaration disabled
    return false;
  }

  bool IsDisabledBySpecial(const Decl * const decl,
                           const StringRef checkerName) {
    string commentString = FormDisablerString(checkerName);
    const StringRef commentStringRef(commentString);
    return IsCommentedWithString(decl, commentStringRef);
  }

  bool IsCommentedWithString(const Decl * const decl,
                             const StringRef commentString)
  {
    if (!decl)
      return false; // invalid decl
    const ASTContext& Context = decl->getASTContext();
    FullComment * Comment = Context.getLocalCommentForDeclUncached(decl);
    if (!Comment)
      return false; // no comment is attached
    ArrayRef<BlockContentComment *> BlockContent = Comment->getBlocks();
    typedef ArrayRef<BlockContentComment *>::const_iterator
      BlockContentIterator;
    BlockContentIterator b = BlockContent.begin();
    BlockContentIterator e = BlockContent.end();
    for (BlockContentIterator i = b; i != e; ++i) {
      const ParagraphComment * const paragraphComment = dyn_cast<const ParagraphComment>(*i);
      if (!paragraphComment)
        continue; // the comment is empty
      typedef clang::comments::Comment::child_iterator CommentIterator;
      CommentIterator child_b = paragraphComment->child_begin();
      CommentIterator child_e = paragraphComment->child_end();
      for (CommentIterator child_i = child_b; child_i != child_e; ++child_i) {
        const TextComment * const textComment = dyn_cast<const TextComment>(*child_i);
        if (!textComment)
          continue;
        if (textComment->isWhitespace())
          continue; // the comment line consists of whitespace only
        const StringRef text = textComment->getText();
        const size_t found = text.find(commentString);
        if (found != npos)
          return true; // the comment line contains the disabling string
      }
    }  
    return false;
  }
} // anonymous namespace
