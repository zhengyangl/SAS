// Author: <unknown>
//         Filip Bartek <filip.bartek@cern.ch> 2013

// Test case for the checker "threadsafety.ConstCast" (1 bug)
// Also triggers the checker "threadsafety.ConstCastAway" (1 bug)


#include <string>


int main()
{
    std::string s = "23";
    std::string const& r_const = s;
    
    // will produce a warning by ConstCastChecker
    // and ConstCastAwayChecker
    std::string & r = const_cast< std::string & >( r_const );

    std::string & r2 = const_cast< std::string & >( r_const ); // bug

    return 0;
}
