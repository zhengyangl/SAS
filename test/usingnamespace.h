// Filip Bartek <filip.bartek@cern.ch>

// Test case for the checker:
// "sas.CodeRules.UsingNamespace"
// `sas::UsingNamespace`

// Bugs: 3


#include <string>
// std::string
// namespace std

using std::string; // bug: using std::*
using namespace std; // bug: using namespace * (`std`)

namespace sas {
  using std::string; // not bug: not in global scope
  using namespace std; // not bug: not in global scope
  int f();
}

using sas::f; // not bug: not in namespace std
using namespace sas; // bug: using namespace * (`sas`)
