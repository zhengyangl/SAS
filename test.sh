#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# Test the plugin on example files.

cd test
#lcg-scan-build -v -v -v -o report -enable-checker threadsafety make -B
lcg-scan-build -o report -enable-checker threadsafety make -B -j 32
cd -
