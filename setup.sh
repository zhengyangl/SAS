#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# This script configures the environment to support comfortable usage of clang static analyzer with sas extension library.
# 1. Adds path to clang binary, scan-build script and scan-view script to PATH.
# 2. Sets lcg-scan-build alias to run scan-build with clang analyzer and sas checker library.

# Locate clang
LLVM_BUILD_DIR=`pwd`/../build
LLVM_BIN_DIR=$LLVM_BUILD_DIR/Release+Asserts/bin

# Add LLVM_BIN_DIR (containing clang binary) to PATH
export PATH=$LLVM_BIN_DIR:$PATH

# Locate scan-build
LLVM_SRC_DIR=`pwd`/../llvm
SCANBUILD_DIR=$LLVM_SRC_DIR/tools/clang/tools/scan-build

# Add SCANBUILD_DIR (containint scan-build script) to PATH
export PATH=$SCANBUILD_DIR:$PATH

# Locate and add scan-view directory to PATH
SCANVIEW_DIR=$LLVM_SRC_DIR/tools/clang/tools/scan-view
export PATH=$SCANVIEW_DIR:$PATH

# Locate sas library and add its location to PATH
SAS_LIB_FILENAME=sas.so
SAS_LIB_DIR=`pwd`/lib
SAS_LIB_PATH=$SAS_LIB_DIR/$SAS_LIB_FILENAME
export PATH=$SAS_LIB_DIR:$PATH

# Let scan-build find the clang and the sas plugin
alias lcg-scan-build="scan-build --use-analyzer `which clang` -load-plugin `which $SAS_LIB_FILENAME`"
