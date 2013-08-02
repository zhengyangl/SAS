//==-- UsingNamespace.cpp - Checks for using namespace and using std:: in headers --------------*- C++ -*--==//
//
// by Shahzad Malik MUZAFFAR [ Shahzad.Malik.MUZAFFAR@cern.ch ]
//
//===----------------------------------------------------------------------===//

#include "UsingNamespace.h"
#include "clang/Basic/SourceManager.h"
#include "SasSupport.h"
using namespace sas;

// using namespace *
void UsingNamespace::checkASTDecl (const clang::UsingDirectiveDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
{
  if (isDeclOK(D,BR)) return;
  reportBug("'using namespace '",D,BR);
}

// using std::*
void UsingNamespace::checkASTDecl (const clang::UsingDecl *D, clang::ento::AnalysisManager &Mgr, clang::ento::BugReporter &BR) const
{
  if (isDeclOK(D,BR)) return;
  std::string NS = D->getQualifier ()->getAsNamespace ()->getNameAsString ();
  if (strcmp(NS.c_str(),"std")!=0) return; // Different from "std"
  reportBug("'using std:: '",D,BR);
}

// Not OK (returns false) when in a header file and top block
bool UsingNamespace::isDeclOK (const clang::NamedDecl *D, clang::ento::BugReporter &BR) const
{
  if (D->getDeclContext ()->getParent()!=0) return true; // Not on top level in AST
  const char *sfile=BR.getSourceManager().getPresumedLoc(D->getLocation ()).getFilename(); // Get filename
  size_t flen = strlen(sfile);
  if ((sfile[flen-2] != '.') || (sfile[flen-1] != 'h')) return true; // Not in a header file (*.h)
  return false;
}

void UsingNamespace::reportBug (const char* bug, const clang::Decl *D, clang::ento::BugReporter &BR) const
{
  clang::ento::PathDiagnosticLocation DLoc = clang::ento::PathDiagnosticLocation::createBegin(D, BR.getSourceManager());
  std::string buf;
  llvm::raw_string_ostream os(buf);
  os << bug << " in headers files.";
  BR.EmitBasicReport(D, "using namespace in header files","CMS code rules",os.str(), DLoc);
}

