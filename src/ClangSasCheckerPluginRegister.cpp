//==                                                                     --==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ]
//    Filip Bartek (2013)
//    Danilo Piparo (2015)
//
//===----------------------------------------------------------------------===//

// To register a new checker class in the SAS library, add a call to
// `registry.addChecker` to the body of the function `clang_registerCheckers`
// in this file. This call needs to have the proper arguments, for example:
// registry.addChecker<sas::MyChecker>("sas.MyCheckerName", "My checker's description");
// This call refers to the class `MyChecker` so you must also make this class
// available in this file by #including the corresponding header file
// (`#include "MyChecker.h"`).

#include "ClassDumper.h"
#include "CatchAll.h"
#include "GlobalAccInCtorChecker.h"
// ThreadSafety
#include "ThreadSafety/Checkers.h"

// General CodingConventions
#include "CodingConventions/General/Rules.h"

// ROOT CodingConventions
#include "CodingConventions/ROOT/Rules.h"

// Performance
#include "Performance/Checkers.h"

// Example
#include "Example/Checkers.h"

#include <clang/StaticAnalyzer/Core/CheckerRegistry.h>

// register all custom checkers with clang
// add new entries here if you want to create a new checker
template<class CHECKER>
void AddToRegistry(clang::ento::CheckerRegistry& registry){
   registry.addChecker<CHECKER>(CHECKER::GetName(),CHECKER::GetDescription());
}

extern "C" void clang_registerCheckers(clang::ento::CheckerRegistry& registry)
{
//    registry.addChecker<sas::ClassDumperCT>("sas.optional.ClassDumperCT", "Dump class info");
//    registry.addChecker<sas::ClassDumperFT>("sas.optional.ClassDumperFT", "Dump class info");
//    registry.addChecker<sas::ClassDumperInherit>("sas.optional.ClassDumperInherit", "Dump class inheritance info");
   registry.addChecker<sas::CatchAll>("sas.CodeRules.CatchAll", "Check for 'catch(...)' in source files");
   registry.addChecker<sas::GlobalAccInCtorChecker>("sas.security.GlobalAccInCtor", "Check for access to global variable in constructor");

   // ThreadSafety
   {
   using namespace sas::ThreadSafety;
   AddToRegistry<ConstCastChecker>(registry);
   AddToRegistry<ConstCastAwayChecker>(registry);
   AddToRegistry<GlobalStaticChecker>(registry);
   AddToRegistry<StaticLocalChecker>(registry);
   AddToRegistry<MutableMemberChecker>(registry);
   }
   // General Coding Conventions and Code Cleanliness
   {
      using namespace sas::CodingConventions::General;
      AddToRegistry<NoUsingNamespaceInHeaders>(registry);
      AddToRegistry<StdPrintoutsChecker>(registry);
   }

   // ROOT Coding Conventions
   {
   using namespace sas::CodingConventions::ROOT;
   AddToRegistry<RN3Checker>(registry);
   AddToRegistry<RN4Checker>(registry);
   AddToRegistry<RN6Checker>(registry);
   AddToRegistry<RN9Checker>(registry);
   AddToRegistry<RN10Checker>(registry);
   AddToRegistry<RN11Checker>(registry);
   AddToRegistry<RN12Checker>(registry);
   AddToRegistry<RN13Checker>(registry);
   AddToRegistry<PtrCastWinChecker>(registry);
   }

   // Performance
   {
   using namespace sas::Performance;
   AddToRegistry<FiniteMathChecker>(registry);
   AddToRegistry<ArgSizeChecker>(registry);
   }
   // Example
   {
   using namespace sas::Example;
   AddToRegistry<VarnameChecker>(registry);
   }

}

extern "C" const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;
