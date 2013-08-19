#!/bin/bash

# Filip Bartek <filip.bartek@cern.ch>

cd ..
source ./setup.sh
cd -

wget -nc http://geant4.cern.ch/support/source/geant4.9.6.p02.tar.gz
tar -zxf geant4.9.6.p02.tar.gz

mkdir -p geant4-build

cd geant4-build
sas-scan-build -v -v -v -o ../report cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX=$OLDPWD/geant4-install ../geant4.9.6.p02
sas-scan-build-sas -o ../report -disable-checker sas.CodeRules.CatchAll make -j 20
cd -


#lcg-scan-build -v -v -v -o $SAS_REPORT_PATH/geant4-cmake cmake -DCMAKE_INSTALL_PREFIX=$OLDPWD/install $OLDPWD/geant4.9.6.p02 -DCMAKE_BUILD_TYPE=Debug
#lcg-scan-build -o $SAS_REPORT_PATH/geant4-make-stable -enable-checker core -enable-checker cplusplus -enable-checker deadcode -enable-checker llvm -enable-checker security -enable-checker unix make -j 20
