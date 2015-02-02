/*
A general interface for coding convention checkers.
EXPERIMENTAL
*/

#ifndef SAS_CHECKERS_CODINGCONVENTIONCHECKER_H
#define SAS_CHECKERS_CODINGCONVENTIONCHECKER_H

#include <clang/StaticAnalyzer/Core/BugReporter/BugReporter.h>
#include <clang/StaticAnalyzer/Core/Checker.h>
#include <clang/Basic/SourceManager.h>
#include <clang/AST/DeclCXX.h>

// What is the meaning of these?
class CommonCCTraits {
public:
      static constexpr const char* BugName="";
      static constexpr const char* BugCategory="";
};

template<class Traits, class WHAT>
class CodingConventionChecker : public clang::ento::Checker<WHAT>{
public:
	// Used for registration
	static const char * GetName(){return Traits::Name;}
	static const char * GetDescription(){return Traits::Description;}
	// Used for reporting
	static const char * GetBugName(){return Traits::BugName;}
	static const char * GetBugCategory(){return Traits::BugCategory;}
	void Report(const clang::Decl* D, const char* msg, clang::ento::BugReporter& BR) const{
		auto DLoc = clang::ento::PathDiagnosticLocation::createBegin(D, BR.getSourceManager());
		// Here filter if comment on line above present or if path is blacklisted
		// ...
		BR.EmitBasicReport(D, this, Traits::BugName, Traits::BugCategory, msg, DLoc);
	}
};
#endif