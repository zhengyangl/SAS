#!/bin/bash

# Author: Filip Bartek <filip.bartek@cern.ch> (2013)

# Test the SAS plugin on example files.

# Requirements:
# sas-scan-build-sas : Runs scan-build with SAS plugin loaded and only SAS
#                      checkers enabled.

cd test

sas-scan-build-sas -o report make -B

cd -
