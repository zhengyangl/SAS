//==                                                                     --==//
//
// by Thomas Hauth [ Thomas.Hauth@cern.ch ] 
//
//===----------------------------------------------------------------------===//



#include "ConstCastChecker.h"
#include "ConstCastAwayChecker.h"
#include "GlobalStaticChecker.h"
#include "StaticLocalChecker.h"
#include "MutableMemberChecker.h"
#include "ClassChecker.h"
#include "ClassDumper.h"
#include "edmChecker.h"
#include "getByChecker.h"
#include "FiniteMathChecker.h"
#include "CatchAll.h"
#include "UsingNamespace.h"
#include "ArgSizeChecker.h"

#include <clang/StaticAnalyzer/Core/CheckerRegistry.h>

// register all custom checkers with clang
// add new entries here if you want to create a new checker
extern "C" 
void clang_registerCheckers ( clang::ento::CheckerRegistry &registry) 
{ 

	registry.addChecker< sas::ConstCastAwayChecker>( "threadsafety.ConstCastAway",  "Checks for casts which remove const qualifier and might result in thread-unsafe code" );
	registry.addChecker< sas::ConstCastChecker>( "threadsafety.ConstCast", "Checks for casts which remove const qualifier and might result in thread-unsafe code" );
	registry.addChecker< sas::StaticLocalChecker>( "threadsafety.StaticLocal", "Checks for non-const method local statics which might not be thread-safe" );
	registry.addChecker< sas::MutableMemberChecker>( "threadsafety.MutableMember", "Checks for members with the mutable keyword which might not be thread-safe" );
	registry.addChecker< sas::GlobalStaticChecker>( "threadsafety.GlobalStatic", "Checks for global non-const statics which might not be thread-safe" );
	registry.addChecker< sas::ClassChecker>( "optional.ClassChecker", "Checks classes of interest" );
	registry.addChecker< sas::ClassDumperCT>( "optional.ClassDumperCT", "Dumps class info" );
	registry.addChecker< sas::ClassDumperFT>( "optional.ClassDumperFT", "Dumps class info" );
	registry.addChecker< sas::ClassDumperInherit>( "optional.ClassDumperInherit", "Dumps class inheritance info" );
	registry.addChecker< sas::FiniteMathChecker>( "cms.NonFiniteMath", "Reports usage of isnan and isinf." );
	registry.addChecker< sas::UsingNamespace>( "cms.CodeRules.UsingNamespace", "Checks for 'using namespace' or 'using std::' in header files" );
	registry.addChecker< sas::CatchAll>( "cms.CodeRules.CatchAll", "Checks for 'catch(...)' in source files" );
	registry.addChecker< sas::edmChecker>( "optional.edmChecker", "Checks classes inheriting from edm::Producer and edm::Filter" );
	registry.addChecker< sas::getByChecker>( "optional.getByChecker", "Checks for calls to edm::getByLabel or edm::getManyByType and reports edm::Handle type passed" );
	registry.addChecker< sas::ArgSizeChecker>( "cms.ArgSize", "Reports args passed by value with size>4k." );
}

extern "C"
const char clang_analyzerAPIVersionString[] = CLANG_ANALYZER_API_VERSION_STRING;




