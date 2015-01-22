//==                                                                     --==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//    Filip Bartek (2013)
//
//===----------------------------------------------------------------------===//

// To register a new checker class in the SAS library, add a call to
// `registry.addChecker` to the body of the function `clang_registerCheckers`
// in this file. This call needs to have the proper arguments, for example:
// registry.addChecker<sas::MyChecker>("sas.MyCheckerName", "My checker's description");
// This call refers to the class `MyChecker` so you must also make this class
// available in this file by #including the corresponding header file
// (`#include "MyChecker.h"`).

#include "ConstCastChecker.h"
#include "ConstCastAwayChecker.h"
#include "GlobalStaticChecker.h"
#include "StaticLocalChecker.h"
#include "MutableMemberChecker.h"
#include "ClassDumper.h"
#include "FiniteMathChecker.h"
#include "CatchAll.h"
#include "UsingNamespace.h"
#include "ArgSizeChecker.h"
#include "VarnameChecker.h"
#include "GlobalAccInCtorChecker.h"

#include <clang/StaticAnalyzer/Core/CheckerRegistry.h>

// register all custom checkers with clang
// add new entries here if you want to create a new checker
extern "C" void clang_registerCheckers(clang::ento::CheckerRegistry& registry)
{
   registry.addChecker<sas::ConstCastAwayChecker>("sas.threadsafety.ConstCastAway", "Check for casts which remove const qualifier");

   registry.addChecker<sas::ConstCastChecker>("sas.threadsafety.ConstCast", "Check for casts which remove const qualifier");

   registry.addChecker<sas::StaticLocalChecker>("sas.threadsafety.StaticLocal", "Check for non-const method local static variables");

   registry.addChecker<sas::MutableMemberChecker>("sas.threadsafety.MutableMember", "Check for members with the mutable keyword");

   registry.addChecker<sas::GlobalStaticChecker>("sas.threadsafety.GlobalStatic", "Check for global non-const static variables");

//    registry.addChecker<sas::ClassDumperCT>("sas.optional.ClassDumperCT", "Dump class info");

//    registry.addChecker<sas::ClassDumperFT>("sas.optional.ClassDumperFT", "Dump class info");

//    registry.addChecker<sas::ClassDumperInherit>("sas.optional.ClassDumperInherit", "Dump class inheritance info");

   registry.addChecker<sas::FiniteMathChecker>("sas.security.NonFiniteMath", "Check for usage of isnan and isinf");

   registry.addChecker<sas::UsingNamespace>("sas.CodeRules.UsingNamespace", "Check for 'using namespace' or 'using std::' in header files");

   registry.addChecker<sas::CatchAll>("sas.CodeRules.CatchAll", "Check for 'catch(...)' in source files");

   registry.addChecker<sas::ArgSizeChecker>("sas.performance.ArgSize", "Check for arguments passed by value with size over 4 KB");

   registry.addChecker<sas::VarnameChecker>("sas.example.Varname", "Report variables whose names don't start with an uppercase letter");

   registry.addChecker<sas::GlobalAccInCtorChecker>("sas.security.GlobalAccInCtor", "Check for access to global variable in constructor");
}

extern "C" const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;
