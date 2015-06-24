// Author: Filip Bartek (2013)

#include "CommentCheckerDisabler.h"

#include <llvm/ADT/StringRef.h>
using llvm::StringRef;
static const size_t& npos = StringRef::npos;

#include <clang/Analysis/AnalysisContext.h>
using clang::LocationContext;

#include <clang/AST/ASTContext.h>
using clang::ASTContext;

#include <clang/AST/Comment.h>
using clang::comments::FullComment;
using clang::comments::BlockContentComment;
using clang::comments::ParagraphComment;
using clang::comments::TextComment;

#include <clang/AST/DeclBase.h>
using clang::Decl;

#include <clang/AST/DeclGroup.h>
using clang::DeclGroupRef;

#include <clang/AST/Stmt.h>
using clang::DeclStmt;
using clang::Stmt;

#include <clang/Basic/SourceManager.h>
using clang::SourceManager;

#include <clang/Basic/SourceLocation.h>
using clang::SourceLocation;
using clang::FileID;

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
using clang::ento::BugReporter;

#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
using clang::ento::AnalysisManager;

#include <clang/StaticAnalyzer/Core/PathSensitive/CheckerContext.h>
using clang::ento::CheckerContext;

#include <llvm/ADT/ArrayRef.h>
using llvm::ArrayRef;

#include <llvm/Support/Casting.h>
using llvm::dyn_cast;

#include <sstream>
using std::ostringstream;

#include <string>
using std::string;

namespace
{
   string FormDisablerString(const StringRef checkerName);

   // Interface
   bool IsDisabledByPreceding(const Stmt* const stmt, CheckerContext& checkerContext, const StringRef checkerName);

   // Implements the main functionality
   bool IsDisabledByPreceding(const SourceLocation& stmtLoc, const SourceManager& sourceManager, const StringRef disablerString);
}

bool sas::IsDisabled(const Stmt* const stmt, CheckerContext& checkerContext, const StringRef checkerName)
{
   if (!stmt) return false;                                                   // Invalid stmt
   if (IsDisabledByPreceding(stmt, checkerContext, checkerName)) return true; // Disabled by preceding line comment
   return false;
}

bool sas::IsDisabled(const Decl* const decl, AnalysisManager& analysisMgr, const StringRef checkerName)
{
   const SourceManager& sourceManager = analysisMgr.getSourceManager();
   return IsDisabled(decl, sourceManager, checkerName);
}

bool sas::IsDisabled(const Decl* const decl, BugReporter& bugReporter, const StringRef checkerName)
{
   const SourceManager& sourceManager = bugReporter.getSourceManager();
   return IsDisabled(decl, sourceManager, checkerName);
}

bool sas::IsDisabled(const Decl* const decl, const SourceManager& sourceManager, const StringRef checkerName)
{
   const SourceLocation sourceLocation = decl->getLocation();
   string disablerString = FormDisablerString(checkerName);
   const StringRef disablerStringRef = StringRef(disablerString);
   return IsDisabledByPreceding(sourceLocation, sourceManager, disablerStringRef);
}

namespace
{
   string FormDisablerString(const StringRef checkerName)
   {
      ostringstream commentOss;
      commentOss << "sas[disable_checker : \"";
      commentOss << checkerName.data();
      commentOss << "\"]";
      return commentOss.str();
   }

   bool IsDisabledByPreceding(const Stmt* const stmt, CheckerContext& checkerContext, const StringRef checkerName)
   {
      if (!stmt) return false; // Invalid stmt
      const SourceManager& sourceManager = checkerContext.getSourceManager();
      const SourceLocation stmtLoc = stmt->getLocStart();
      const string disablerString = FormDisablerString(checkerName);
      const StringRef disablerStringRef = StringRef(disablerString);
      return IsDisabledByPreceding(stmtLoc, sourceManager, disablerStringRef);
   }

   bool IsDisabledByPreceding(const SourceLocation& stmtLoc, const SourceManager& sourceManager, const StringRef disablerString)
   {
      const unsigned stmtLineNum = sourceManager.getSpellingLineNumber(stmtLoc);
      if (stmtLineNum <= 1)
         return false;
      const FileID fileID = sourceManager.getFileID(stmtLoc);

      const unsigned Line = stmtLineNum - 1;
      const unsigned beginCol = 1;
      const unsigned endCol = 0;
      // When the argument 'Col' is 0,
      // according to http://clang.llvm.org/doxygen/SourceManager_8cpp_source.html#l01761 ,
      // A SourceLocation of Last character of the line will be returned.
      // This is not well documented in Clang documentation.
      const SourceLocation locBegin = sourceManager.translateLineCol(fileID, Line, beginCol);
      const SourceLocation locEnd = sourceManager.translateLineCol(fileID, Line, endCol);
      const char* begin = sourceManager.getCharacterData(locBegin);
      const char* end = sourceManager.getCharacterData(locEnd);
      assert(end >= begin);
      const string lineString = string(begin, end - begin);
      const StringRef lineStringRef = StringRef(lineString);
      const size_t commentCol = lineStringRef.find("//");
      if (commentCol == npos) return false; /// No `//` comment on this line
      const StringRef commentContent = lineStringRef.substr(commentCol + 2);
      const size_t disablerCol = commentContent.find(disablerString);
      if (disablerCol == npos) return false; /// No disabler in this comment
      /// Bingo!
      return true;
   }
} // anonymous namespace
