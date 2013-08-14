#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

cd ..
source ./setup.sh
cd -

git clone http://root.cern.ch/git/root.git

mkdir -p root

cd root
git checkout v5-34-09
cd -

mkdir -p root-build

cd root-build
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=`which c++-analyzer` -DCMAKE_C_COMPILER=`which ccc-analyzer` ../root
sas-scan-build-sas -o ../report -disable-checker sas.Varname make -j
cd -
