#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# This script configures the environment to support comfortable usage of clang static analyzer with sas extension library.
# 1. Adds path to clang binary, scan-build script and scan-view script to PATH.
# 2. Sets sas-scan-build alias to run scan-build with clang analyzer and sas checker library.

# Locate SAS build directory
SAS_BUILD_DIR=`pwd`/../sas-build

# Locate LLVM directories
LLVM_BUILD_DIR=`pwd`/../build
LLVM_SRC_DIR=`pwd`/../llvm


# Locate clang binary directory and add it to PATH
LLVM_BIN_DIR=$LLVM_BUILD_DIR/Release+Asserts/bin
export PATH=$LLVM_BIN_DIR:$PATH

# Locate scan-build script directory and add it to PATH
SCANBUILD_DIR=$LLVM_SRC_DIR/tools/clang/tools/scan-build
export PATH=$SCANBUILD_DIR:$PATH

# Locate scan-view script directory and add it to PATH
SCANVIEW_DIR=$LLVM_SRC_DIR/tools/clang/tools/scan-view
export PATH=$SCANVIEW_DIR:$PATH

# Locate sas library
SAS_LIB_FILENAME=sas.so
SAS_LIB_DIR=$SAS_BUILD_DIR/lib
SAS_LIB_PATH=$SAS_LIB_DIR/$SAS_LIB_FILENAME


# Let scan-build find the clang and the sas plugin
alias sas-scan-build="scan-build --use-analyzer `which clang` -load-plugin $SAS_LIB_PATH"

# scan-build with all checkers disabled
SCANBUILD_ARG_DISABLE_CHECKERS="-disable-checker alpha -disable-checker core -disable-checker cplusplus -disable-checker deadcode -disable-checker debug -disable-checker llvm -disable-checker osx -disable-checker security -disable-checker unix"
alias sas-scan-build-none="sas-scan-build $SCANBUILD_ARG_DISABLE_CHECKERS"

# scan-build with only SAS checkers enabled
SCANBUILD_ARG_ENABLE_SAS_CHECKERS="-enable-checker sas -enable-checker threadsafety -enable-checker performance -enable-checker security.GlobalAccInCtor -disable-checker sas.example"
alias sas-scan-build-sas="sas-scan-build-none $SCANBUILD_ARG_ENABLE_SAS_CHECKERS"

SCANBUILD_ARG_ENABLE_ALL_CHECKERS="-enable-checker alpha -enable-checker core -enable-checker cplusplus -enable-checker deadcode -enable-checker llvm -enable-checker osx -enable-checker security -enable-checker unix"
alias sas-scan-build-all="sas-scan-build-sas $SCANBUILD_ARG_ENABLE_ALL_CHECKERS"
