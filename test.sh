#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# Test the plugin on example files.

cd test

# All sas checkers (both threadsafe.* and sas.*):
#sas-scan-build -o report -enable-checker threadsafety -enable-checker sas make -B -j 32

# Only threadsafe.* checkers:
sas-scan-build -o report -enable-checker threadsafety make -B

cd -
