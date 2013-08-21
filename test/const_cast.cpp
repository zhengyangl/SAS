// Author: <unknown>
//         Filip Bartek <filip.bartek@cern.ch> 2013

// Test case for the checker "threadsafety.ConstCast" (2 bugs)
// Also triggers the checker "threadsafety.ConstCastAway" (2 bugs)

#include <string>
using std::string;

int main()
{
  const string& r_const = "23";
    
  string& r1 = const_cast<string&>(r_const); // bug
  string& r2 = const_cast<string&>(r_const); // bug

  return 0;
}
