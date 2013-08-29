#!/bin/bash

# Author: Filip Bartek <filip.bartek@cern.ch> (2013)

# This script configures the environment to support comfortable usage of Clang
# Static Analyzer and Clang compiler by adding the paths to the respective
# binary and scripts to PATH environment variable.

# Requires:
# LLVM_SRC_DIR : Absolute path to LLVM
# LLVM_SRC_BUILD_DIR : Absolute path to LLVM build

# Require LLVM_SRC_DIR
if [ ! "${LLVM_SRC_DIR+set}" = set ]; then
    echo "LLVM_SRC_DIR must be set!"
    echo "Set LLVM_SRC_DIR to absolute path to LLVM."
    echo "Example usage: LLVM_SRC_DIR=/path/to/llvm LLVM_SRC_BUILD_DIR=/path/to/llvm/build source ./setup-path.sh"
    return 1
fi

# Require LLVM_SRC_BUILD_DIR
if [ ! "${LLVM_SRC_BUILD_DIR+set}" = set ]; then
    echo "LLVM_SRC_BUILD_DIR must be set!"
    echo "Set LLVM_SRC_BUILD_DIR to absolute path to LLVM build."
    echo "Example usage: LLVM_SRC_DIR=/path/to/llvm LLVM_SRC_BUILD_DIR=/path/to/llvm/build source ./setup-path.sh"
    return 1
fi

# Locate clang binary directory and add it to PATH
LLVM_BIN_DIR=$LLVM_SRC_BUILD_DIR/Release+Asserts/bin
export PATH=$LLVM_BIN_DIR:$PATH

# Locate scan-build script directory and add it to PATH
SCANBUILD_DIR=$LLVM_SRC_DIR/tools/clang/tools/scan-build
export PATH=$SCANBUILD_DIR:$PATH

# Locate scan-view script directory and add it to PATH
SCANVIEW_DIR=$LLVM_SRC_DIR/tools/clang/tools/scan-view
export PATH=$SCANVIEW_DIR:$PATH
