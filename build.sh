#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# This script builds sas plugin library for clang static analyzer.

# Locate SAS build directory
SAS_BUILD_DIR_REL=../sas-build


# Go to SAS build directory
mkdir -p $SAS_BUILD_DIR_REL
cd $SAS_BUILD_DIR_REL

# Configure the build
cmake $OLDPWD

# Build the library
make -j 32

# Return to .
cd -
