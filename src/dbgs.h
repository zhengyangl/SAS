// -*-c++-*-

// Author: Filip Bartek (2013)

// Defines a `raw_ostream` for debug output.
// Access the `ostream` as `sas::dbgs()`.
// Use it the same way as (for example) `llvm::outs()`, namely:
//   dbgs() << "Hello world!\n";

// If the macro NDEBUG is defined, `dbgs` redirects the stream to standard
// output. Otherwise, it redirects the stream to `nulls`, a null stream (dumping
// the data).

#ifndef SAS_DBGS_H
#define SAS_DBGS_H

// Declarations:

#include <llvm/Support/raw_ostream.h>
// llvm::raw_ostream

namespace sas
{
   static llvm::raw_ostream& dbgs();
} // end namespace sas

// Definitions:

#include <llvm/Support/raw_ostream.h>
// llvm::outs
// llvm::nulls

llvm::raw_ostream& sas::dbgs()
{
#ifdef NDEBUG
   return llvm::nulls();
#else
   return llvm::outs();
#endif // NDEBUG
}

#endif
