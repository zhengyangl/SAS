// Filip Bartek <filip.bartek@cern.ch>

// Test case for the checker sas.Varname
// Bugs: 1

int main(void) {
  int variable; // bug: doesn't begin with uppercase
  int Variable; // not bug: begins with uppercase
  return 0;
}
