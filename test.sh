#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# Test the plugin on example files.

cd test

# All sas checkers (both threadsafe. and cms.):
#lcg-scan-build -o report -enable-checker threadsafety -enable-checker cms make -B -j 32

# Only threadsafe. sas checkers:
lcg-scan-build -o report -enable-checker threadsafety make -B -j 32

cd -
