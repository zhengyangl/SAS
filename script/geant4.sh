#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

cd ..
source ./setup.sh
cd -

wget -nc http://geant4.cern.ch/support/source/geant4.9.6.p02.tar.gz
tar -zxf geant4.9.6.p02.tar.gz

mkdir -p geant4-build

cd geant4-build
sas-scan-build-none -o ../report cmake -DCMAKE_BUILD_TYPE=Debug ../geant4.9.6.p02
sas-scan-build-sas -o ../report -disable-checker sas.Varname make -j
cd -
