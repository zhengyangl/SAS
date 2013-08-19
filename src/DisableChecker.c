#include "DisableChecker.h"

#include <clang/AST/DeclBase.h>
using clang::Decl;

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;

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

#define DEBUG

#ifdef DEBUG
#include <llvm/Support/raw_ostream.h>
using llvm::outs; // standard output
#endif // DEBUG

_Bool isDisabled(const Decl * const D, const StringRef& CheckerName)
{
  const ASTContext& Context = D->getASTContext();
  FullComment * Comment = Context.getLocalCommentForDeclUncached(D);
  if (!Comment)
    return false; // D has no associated comment
  ArrayRef<BlockContentComment *> BlockContent = Comment->getBlocks();
  typedef ArrayRef<BlockContentComment *>::const_iterator BlockContentIterator;
  BlockContentIterator b = BlockContent.begin();
  BlockContentIterator e = BlockContent.end();
  for (BlockContentIterator i = b; i != e; ++i) {
    const ParagraphComment * const paragraphComment = dyn_cast<const ParagraphComment>(*i);
    if (!paragraphComment)
      continue;
    typedef clang::comments::Comment::child_iterator CommentIterator;
    CommentIterator child_b = paragraphComment->child_begin();
    CommentIterator child_e = paragraphComment->child_end();
    for (CommentIterator child_i = child_b; child_i != child_e; ++child_i) {
      const TextComment * const textComment = dyn_cast<const TextComment>(*child_i);
      if (!textComment)
        continue;
#ifdef DEBUG
      outs() << "  " << textComment->getText() << "\n";
      outs() << "  " << textComment->isWhitespace() << "\n";
#endif // DEBUG
    }
  }
  
  return false;
}
