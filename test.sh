#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# Test the plugin on example files.

SCANBUILD_ARG_DISABLE_CHECKERS=-disable-checker core -disable-checker cplusplus -disable-checker deadcode -disable-checker llvm -disable-checker osx -disable-checker security -disable-checker unix

cd test

# All sas checkers (both threadsafe.* and sas.*):
#sas-scan-build -o report $SCANBUILD_ARG_DISABLE_CHECKERS -enable-checker threadsafety -enable-checker sas make -B -j 32

# Only threadsafe.* checkers:
#sas-scan-build -o report $SCANBUILD_ARG_DISABLE_CHECKERS -enable-checker threadsafety make -B -j 32

# threadsafe.* and sas.Varname
#sas-scan-build -o report $SCANBUILD_ARG_DISABLE_CHECKERS -enable-checker threadsafety -enable-checker sas make -B -j 32

# sas-scan-build-sas
sas-scan-build-sas -o report make -B -j 32

cd -
