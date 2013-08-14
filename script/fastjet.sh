#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

# This script fetches fastjet source code and runs static analysis on it with
# the SAS checkers enabled.

cd ..
source ./setup.sh
cd -

wget -nc http://fastjet.fr/repo/fastjet-3.0.4.tar.gz
tar -zxf fastjet-3.0.4.tar.gz

cd fastjet-3.0.4
sas-scan-build-none -o ../report ./configure --enable-debug
sas-scan-build-sas -o ../report -disable-checker sas.Varname make -j
cd -
