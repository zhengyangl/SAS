// -*-c++-*-

// Author: Filip Bartek (2013)

// See "checkerdisabler.cpp" for documentation.

// Bugs: 2

#include <string>
// std::string
// namespace std

// sas[disable_checker : "sas.CodeRules.UsingNamespace"]
using std::string; // not bug (UsingNamespace): disabled by comment
// sas[disable_checker : "sas.CodeRules.UsingNamespace"]
using namespace std; // not bug (UsingNamespace): disabled by comment

using std::string; // bug (UsingNamespace): using std::*
using namespace std; // bug (UsingNamespace): using namespace * (`std`)
