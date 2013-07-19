#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# This script builds sas plugin library for clang static analyzer.
# The build is not incremental as this script is intended for testing purposes.

cmake .
make -j 32
