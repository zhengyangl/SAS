// Filip Bartek <filip.bartek@cern.ch>

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

namespace {
  bool IsCommentedWithString(const Decl * const decl,
                             const StringRef commentString);
}

bool sas::IsDisabled(const Decl * const decl,
                     const StringRef checkerName) {
  ostringstream commentOss;
  commentOss << "sas[disable_checker : \"";
  commentOss << checkerName.data();
  commentOss << "\"]";
  string commentString = commentOss.str();
  const StringRef commentStringRef(commentString);
  return IsCommentedWithString(decl, commentStringRef);
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
                     SourceManager& sourceManager,
                     const StringRef checkerName)
{
  if (!stmt)
    return false; // Invalid stmt
  // TODO: Implement.
  stmt->dump(sourceManager);
  //SourceRange stmt->getSourceRange();
  //SourceLocation stmt->getLocStart();
  return false;
}

namespace {
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
