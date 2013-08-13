//==                                                                     --==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ] 
//    Filip Bartek [ filip.bartek@cern.ch ]
//
//===----------------------------------------------------------------------===//

// To register a new checker class in the SAS library, add a call to `registry.addChecker` to the body of the function `clang_registerCheckers`. This call needs to have the proper arguments, for example:
// registry.addChecker<sas::MyChecker>("sas.MyCheckersName", "My checker's description");
// This call refers to the class `MyChecker` so you must also make this class available in this file by #including the corresponding header file (`#include "MyChecker.h"`).

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

#include <clang/StaticAnalyzer/Core/CheckerRegistry.h>

// register all custom checkers with clang
// add new entries here if you want to create a new checker
extern "C" 
void clang_registerCheckers ( clang::ento::CheckerRegistry &registry) 
{ 
  registry.addChecker<sas::ConstCastAwayChecker>("threadsafety.ConstCastAway",
						 "Checks for casts which remove const qualifier and might result in thread-unsafe code");
  registry.addChecker<sas::ConstCastChecker>("threadsafety.ConstCast",
					     "Checks for casts which remove const qualifier and might result in thread-unsafe code");
  registry.addChecker<sas::StaticLocalChecker>("threadsafety.StaticLocal",
					       "Checks for non-const method local statics which might not be thread-safe");
  registry.addChecker<sas::MutableMemberChecker>("threadsafety.MutableMember",
						 "Checks for members with the mutable keyword which might not be thread-safe");
  registry.addChecker<sas::GlobalStaticChecker>("threadsafety.GlobalStatic",
						"Checks for global non-const statics which might not be thread-safe");
  registry.addChecker<sas::ClassDumperCT>("optional.ClassDumperCT",
					  "Dumps class info");
  registry.addChecker<sas::ClassDumperFT>("optional.ClassDumperFT",
					  "Dumps class info");
  registry.addChecker<sas::ClassDumperInherit>("optional.ClassDumperInherit",
					       "Dumps class inheritance info");
  registry.addChecker<sas::FiniteMathChecker>("sas.NonFiniteMath", "Reports usage of isnan and isinf.");
  registry.addChecker<sas::UsingNamespace>("sas.CodeRules.UsingNamespace",
					   "Checks for 'using namespace' or 'using std::' in header files");
  registry.addChecker<sas::CatchAll>("sas.CodeRules.CatchAll",
				     "Checks for 'catch(...)' in source files");
  registry.addChecker<sas::ArgSizeChecker>("sas.ArgSize",
					   "Reports args passed by value with size>4k.");
  registry.addChecker<sas::VarnameChecker>("sas.Varname",
					   "Reports variables whose names don't start with an uppercase letter.");
}

extern "C"
const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;
