// Test case for the checker "sas.example.Varname"
// Tests selective disabling of the checker using formatted comments

// Filip Bartek <filip.bartek@cern.ch>

// Bugs: 2

int main(void) {
  int variable; // bug: doesn't begin with uppercase
  int Variable; // not bug: begins with uppercase

  /// <!-- sas::disable_checker("sas.example.Varname") -->
  int var0; // not bug: checker disabled by comment

  /** sas::disable_checker("sas.example.Varname") */
  int var1; // not bug: checker disabled by comment

  /// comment start
  /// sas::disable_checker("sas.example.Varname")
  /// comment end
  int var2; // not bug: checker disabled by comment

  /** comment start
      sas::disable_checker("sas.example.Varname")
      comment end */
  int var3; // not bug: checker disabled by comment

  /// disable_checker("sas.example.Varname")
  int var4; // bug: disable comment string not fully qualified

  return 0;
}
