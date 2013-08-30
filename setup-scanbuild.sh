#!/bin/bash

# Author: Filip Bartek <filip.bartek@cern.ch> (2013)

# This script configures the environment to support comfortable usage of Clang
# Static Analyzer with SAS plugin.
# It sets the following aliases:
# sas-scan-build
# sas-scan-build-none
# sas-scan-build-sas
# sas-scan-build-all

# Requirements:
# SAS_LIB_PATH : For example ~/sas-build/lib/sas.so
# clang in PATH
# scan-build in PATH

# Require SAS_LIB_PATH
if [ ! "${SAS_LIB_PATH+set}" = set ]; then
    echo "SAS_LIB_PATH must be set!"
    echo "Example usage: SAS_LIB_PATH=~/sas-build/lib/sas.so source ./setup.sh"
    return 1
fi

# scan-build with correct analyzer and SAS plugin loaded
alias sas-scan-build="scan-build --use-analyzer `which clang` -load-plugin $SAS_LIB_PATH"

# scan-build with all checkers disabled
SCANBUILD_ARG_DISABLE_CHECKERS="-disable-checker alpha -disable-checker core -disable-checker cplusplus -disable-checker deadcode -disable-checker debug -disable-checker llvm -disable-checker osx -disable-checker security -disable-checker unix"
alias sas-scan-build-none="sas-scan-build $SCANBUILD_ARG_DISABLE_CHECKERS"

# scan-build with only SAS checkers enabled
SCANBUILD_ARG_ENABLE_SAS_CHECKERS="-enable-checker sas -disable-checker sas.optional -disable-checker sas.example"
alias sas-scan-build-sas="sas-scan-build-none $SCANBUILD_ARG_ENABLE_SAS_CHECKERS"

# scan-build with all checkers enabled (stable, alpha, osx, unix, SAS)
SCANBUILD_ARG_ENABLE_ALL_CHECKERS="-enable-checker alpha -enable-checker core -enable-checker cplusplus -enable-checker deadcode -enable-checker llvm -enable-checker osx -enable-checker security -enable-checker unix"
alias sas-scan-build-all="sas-scan-build-sas $SCANBUILD_ARG_ENABLE_ALL_CHECKERS"
