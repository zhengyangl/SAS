// Filip Bartek <filip.bartek@cern.ch>

// Test case for the checker:
// "sas.CodeRules.UsingNamespace"
// `sas::UsingNamespace`

// Bugs: 0


#include "usingnamespace.h" // Bugs: >0

#include <string>
// std::string
// namespace std

using std::string; // not bug: not in header file
using namespace std; // not bug: not in header file

int main(void) {
  return 0;
}
