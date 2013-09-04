// Author: Filip Bartek (2013)

// Test case for the checker disabler
// Tests selective disabling of the checkers using formatted comments
// Uses checkers:
// - sas.example.Varname
// - sas.CodeRules.UsingNamespace
// - sas.threadsafety.ConstCast

#include "checkerdisabler.h"

#include <string>
using std::string;

int main(void)
{
  int variable; // bug (Varname): doesn't begin with uppercase
  int Variable; // not bug (Varname): begins with uppercase

  /// sas[disable_checker : "sas.example.Varname"]
  int varSlashSlashSingle; // not bug (Varname): disabled by /// comment

  /** sas[disable_checker : "sas.example.Varname"] */
  int varAsteriskAsteriskSingle; // bug (Varname): not disabled by /***/ comment

  //! sas[disable_checker : "sas.example.Varname"]
  int varSlashExclamationSingle; // not bug (Varname): disabled by //! comment

  /*! sas[disable_checker : "sas.example.Varname"] */
  int varAsteriskExclamationSingle; // bug (Varname): not disabled by /*!*/ comment

  /// begin
  /// sas[disable_checker : "sas.example.Varname"]
  /// end
  int varSlashSlashMulti; // bug: disabled too high

  /** begin
      sas[disable_checker : "sas.example.Varname"]
      end */
  int varAsteriskAsteriskMulti; // bug: not disabled by comment

  //! begin
  //! sas[disable_checker : "sas.example.Varname"]
  //! end
  int varSlashExclamationMulti; // bug: not disabled by comment

  /*! begin
    sas[disable_checker : "sas.example.Varname"]
    end */
  int varAsteriskExclamationMulti; // bug: not disabled by comment

  /// [disable_checker : "sas.example.Varname"]
  int varNotQualified; // bug: disable comment string not fully qualified

  /// <!-- sas[disable_checker : "sas.example.Varname"] -->
  int varDoxygenComment; // not bug: HTML comment markup

  // sas[disable_checker : "sas.example.Varname"]
  int varCommonSlash; // not bug: disabler in common comment (not special)

  /* sas[disable_checker : "sas.example.Varname"] */
  int varCommonAsterisk; // bug (Varname): disabler in common * comment (not special)

  const string& S_const = "s";
  string& S = const_cast<string&>(S_const); // bug (ConstCast): const_cast used
  // sas[disable_checker : "threadsafety.ConstCast"]
  string& SDis = const_cast<string&>(S_const); // not bug (ConstCast): disabled by comment

  return 0;
}

void f1(void)
{
  const string& S_const = "s";
  string& S = const_cast<string&>(S_const); // bug: const_cast used
  return;
}

void f2(void)
{
  const string& S_const = "s";
  // sas[disable_checker : "threadsafety.ConstCast"] sas[disable_checker : "sas.example.Varname"]
  string& s = const_cast<string&>(S_const); // not bug: disabled by comment
  return;
}
