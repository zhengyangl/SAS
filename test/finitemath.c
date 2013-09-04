// Author: Filip Bartek (2013)

// Test case for checker sas.security.NonFiniteMath

// Bugs: 2

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
