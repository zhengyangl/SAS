// Author: <unknown>
//         Filip Bartek <filip.bartek@cern.ch> 2013

// Test case for the checker "threadsafety.ConstCast" (2 bugs)
// Also triggers the checker "threadsafety.ConstCastAway" (2 bugs)

#include <string>
std::string;

int main()
{
  string s = "23";
  string const& r_const = s;
    
  // will produce a warning by ConstCastChecker
  // and ConstCastAwayChecker
  string & r = const_cast< string & >( r_const );
  string & r2 = const_cast< string & >( r_const ); // bug

  return 0;
}
