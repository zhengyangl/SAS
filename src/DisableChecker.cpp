// Filip Bartek <filip.bartek@cern.ch>

#include "DisableChecker.h"

#include <clang/AST/DeclBase.h>
using clang::Decl;

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

namespace {
  bool IsCommentedWithString(const clang::Decl * const decl,
                             const llvm::StringRef commentString);
}

bool sas::IsDisabled(const clang::Decl * const decl,
                     const llvm::StringRef checkerName) {
  ostringstream commentOss;
  commentOss << "sas::disable_checker(\"";
  commentOss << checkerName.data();
  commentOss << "\")";
  string commentString = commentOss.str();
  const StringRef commentStringRef(commentString);
  return IsCommentedWithString(decl, commentStringRef);
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
      return false; // decl has no associated special comment
    ArrayRef<BlockContentComment *> BlockContent = Comment->getBlocks();
    typedef ArrayRef<BlockContentComment *>::const_iterator BlockContentIterator;
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
          return true;
      }
    }  
    return false;
  }
} // anonymous namespace
