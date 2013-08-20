// Author: Filip Bartek <filip.bartek@cern.ch>

// Test case for the checker disabler IsDisabled
// Tests selective disabling of the checkers using formatted comments
// Uses checkers:
// - sas.example.Varname
// - sas.CodeRules.UsingNamespace

// Bugs: 2

#include "checkerdisabler.h"

int main(void) {
  int variable; // bug: doesn't begin with uppercase
  int Variable; // not bug: begins with uppercase

  /// sas::disable_checker("sas.example.Varname")
  int varSlashSlashSingle; // not bug: checker disabled by comment

  /** sas::disable_checker("sas.example.Varname") */
  int varAsteriskAsteriskSingle; // not bug: checker disabled by comment

  //! sas::disable_checker("sas.example.Varname")
  int varSlashExclamationSingle; // not bug: checker disabled by comment

  /*! sas::disable_checker("sas.example.Varname") */
  int varAsteriskExclamationSingle; // not bug: checker disabled by comment

  /// begin
  /// sas::disable_checker("sas.example.Varname")
  /// end
  int varSlashSlashMulti; // not bug: checker disabled by comment

  /** begin
      sas::disable_checker("sas.example.Varname")
      end */
  int varAsteriskAsteriskMulti; // not bug: checker disabled by comment

  //! begin
  //! sas::disable_checker("sas.example.Varname")
  //! end
  int varSlashExclamationMulti; // not bug: checker disabled by comment

  /*! begin
    sas::disable_checker("sas.example.Varname")
    end */
  int varAsteriskExclamationMulti; // not bug: checker disabled by comment

  /// disable_checker("sas.example.Varname")
  int varNotQualified; // bug: disable comment string not fully qualified

  /// <!-- sas::disable_checker("sas.example.Varname") -->
  int varDoxygenComment; // not bug: HTML comment markup

  return 0;
}
