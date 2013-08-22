// Author: Filip Bartek <filip.bartek@cern.ch> (2013)

#include "StaticAccInCtorChecker.h"

using clang::ento::CallEvent;
using clang::ento::CheckerContext;

namespace sas {
  const char * const StaticAccInCtorChecker::checkerName =
    "security.StaticAccInCtor";

  void StaticAccInCtorChecker::checkPreCall(const CallEvent &Call,
                                            CheckerContext &C) const
  {
  }
} // end namespace sas
