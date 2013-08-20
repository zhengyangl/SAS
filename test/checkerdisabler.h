// Author: Filip Bartek <filip.bartek@cern.ch>

// Bugs: 2

#include <string>
// std::string
// namespace std

/// sas[disable_checker : "sas.CodeRules.UsingNamespace"]
using std::string; // not bug: disabled by comment
/// sas[disable_checker : "sas.CodeRules.UsingNamespace"]
using namespace std; // not bug: disabled by comment

using std::string; // bug: using std::*
using namespace std; // bug: using namespace * (`std`)
