// Filip Bartek <filip.bartek@cern.ch>

// A test case for the checker "sas.NonFiniteMath"

// Bugs: 2

// Warning: Doesn't work properly on my computer as <math.h> doesn't seem to
// define isnan and isinf as functions.


#include <math.h>
// isnan
// isinf

int main(void) {
  int Result = 0;
  float Var = 0.0f;
  if (isnan(Var)) { // bug: isnan used
    Result = 1;
  }
  if (isinf(Var)) { // bug: isinf used
    Result = 2;
  }
  return Result;
}
