// Author: Danilo Piparo (2015)

#include "PtrCastWinChecker.h"

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/BugReporter/PathDiagnostic.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/StaticAnalyzer/Core/PathSensitive/AnalysisManager.h>
#include <llvm/ADT/StringRef.h>

namespace sas
{
   namespace CodingConventions
   {
      namespace ROOT
      {
         void PtrCastWinChecker::checkPreStmt(const clang::CStyleCastExpr* CE, clang::ento::CheckerContext& C) const
         {
            if (clang::CK_PointerToIntegral != CE->getCastKind()) return;
            auto subExpr = CE->getSubExpr();
            auto& Ctx = C.getASTContext();
            clang::QualType fromQType = Ctx.getCanonicalType(subExpr->getType());
            clang::QualType toQType = Ctx.getCanonicalType(CE->getType());
            if (fromQType->isPointerType() || fromQType->isArrayType()) {
               auto toType = toQType.getTypePtr();
               // Case one: the toType is a builtin and not a long long
               if (toType->isSpecificBuiltinType(clang::BuiltinType::Long) ||
                   toType->isSpecificBuiltinType(clang::BuiltinType::ULong)){
                   std::string r = "Casting pointers to integer types which are not (unsigned) long long (in this case a ";
                   r+=toQType.getAsString ();
                   r+=") is wrong on Windows 64 bits. ";
                   Report(CE, r.c_str(), C);
               }
            }
         }
      } // end of ns root
   } // end ns cc
} // end namespace sas
