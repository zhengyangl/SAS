#!/bin/bash -x

export LC_CTYPE=en_US.UTF-8
export LC_ALL=en_US.UTF-8


THIS=$(dirname ${BASH_SOURCE[0]})



# first arguments is the source directory
if [ $# -ge 3 ]; then
  LABEL=$1 ; shift
  COMPILER=$1 ; shift
  BUILDTYPE=$1 ; shift
else
  echo "$0: expecting 3 arguments [LABEL] [COMPILER] [BUILDTYPE]"
  return
fi

if [ $LABEL == slc6 ] || [ $LABEL == cc7 ]
then
  export PATH=/afs/cern.ch/sw/lcg/contrib/CMake/3.0.0/Linux-i386/bin:${PATH}
else
  export EXTERNALDIR=$HOME/ROOT-externals/
fi

if [[ $COMPILER == *gcc* ]]
then
  gcc47version=4.7
  gcc48version=4.8
  gcc49version=4.9
  COMPILERversion=${COMPILER}version

  ARCH=$(uname -m)
  . /afs/cern.ch/sw/lcg/contrib/gcc/${!COMPILERversion}/${ARCH}-${LABEL}/setup.sh
  export FC=gfortran
  export CXX=`which g++`
  export CC=`which gcc`
fi

if [[ $COMPILER == *clang* ]]
then
  clang36version=3.6
  COMPILERversion=${COMPILER}version

  . /afs/cern.ch/sw/lcg/external/llvm/${!COMPILERversion}/${ARCH}-${LABEL}/setup.sh
fi

#---need add correct compiler settings for clang? --------

export CMAKE_SOURCE_DIR=$WORKSPACE
export CMAKE_BINARY_DIR=$WORKSPACE/builds
export CMAKE_BUILD_TYPE=$BUILDTYPE
export CTEST_BUILD_OPTIONS=""
export LABEL
