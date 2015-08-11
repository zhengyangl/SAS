cmake_minimum_required(VERSION 2.8)

######################################################
# Build name settings
find_program(UNAME NAMES uname)
macro(getuname name flag)
  exec_program("${UNAME}" ARGS "${flag}" OUTPUT_VARIABLE "${name}")
endmacro(getuname)

getuname(osname -s)
getuname(osrel  -r)
getuname(cpu    -m)

set(CTEST_BUILD_NAME "${osname}-${cpu}")
if(DEFINED ENV{LABEL})
  set(CTEST_BUILD_NAME "${CTEST_BUILD_NAME}-$ENV{LABEL}")
endif(DEFINED ENV{LABEL})
if(DEFINED ENV{CMAKE_BUILD_TYPE})
  set(CTEST_BUILD_NAME "${CTEST_BUILD_NAME}-$ENV{CMAKE_BUILD_TYPE}")
endif(DEFINED ENV{CMAKE_BUILD_TYPE})
message("CTEST name: ${CTEST_BUILD_NAME}")

find_program(HOSTNAME_CMD NAMES hostname)
exec_program(${HOSTNAME_CMD} ARGS OUTPUT_VARIABLE HOSTNAME)
IF(NOT DEFINED CTEST_SITE)
  SET(CTEST_SITE "${HOSTNAME}")
ENDIF(NOT DEFINED CTEST_SITE)

######################################################
# CTest/CMake
set(CTEST_BUILD_CONFIGURATION "$ENV{CMAKE_BUILD_TYPE}")
set(CTEST_SOURCE_DIRECTORY "$ENV{CMAKE_SOURCE_DIR}")
set(CTEST_BINARY_DIRECTORY "$ENV{CMAKE_BINARY_DIR}")
set(CTEST_CMAKE_GENERATOR "Unix Makefiles")
set(CTEST_BUILD_OPTIONS "$ENV{CTEST_BUILD_OPTIONS}")
set(CTEST_CONFIGURE_COMMAND "\"${CMAKE_COMMAND}\" \"-G${CTEST_CMAKE_GENERATOR}\" -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} -DCMAKE_BUILD_TYPE=${CTEST_BUILD_CONFIGURATION} ${CTEST_BUILD_OPTIONS} ${CTEST_SOURCE_DIRECTORY}")
ctest_empty_binary_directory(${CTEST_BINARY_DIRECTORY})

#######################################################
# Build dashboard model setup
SET(MODEL Nightly)
IF(${CTEST_BUILD_CONFIGURATION} MATCHES Experimental)
  SET(MODEL Experimental)
ENDIF(${CTEST_BUILD_CONFIGURATION} MATCHES Experimental)
IF(${CTEST_BUILD_CONFIGURATION} MATCHES Continuous)
  SET(MODEL Continuous)
ENDIF(${CTEST_BUILD_CONFIGURATION} MATCHES Continuous)

find_program(CTEST_COMMAND_BIN NAMES ctest)
SET (CTEST_COMMAND
  "$CTEST_COMMAND_BIN -D ${MODEL}")

#######################################################
# Git update configurations
find_program(CTEST_GIT_COMMAND NAMES git)
if(NOT EXISTS "${CTEST_SOURCE_DIRECTORY}")
  set(CTEST_CHECKOUT_COMMAND "${CTEST_GIT_COMMAND} clone http://github.com/dpiparo/SAS.git ${CTEST_SOURCE_DIRECTORY}")
endif()
set(CTEST_GIT_UPDATE_COMMAND "${CTEST_GIT_COMMAND}")

if(NOT "$ENV{GIT_COMMIT}" STREQUAL "")
   set(CTEST_CHECKOUT_COMMAND "cmake -E chdir ${CTEST_SOURCE_DIRECTORY} ${CTEST_GIT_COMMAND} checkout -f $ENV{GIT_PREVIOUS_COMMIT}")
   set(CTEST_GIT_UPDATE_CUSTOM  ${CTEST_GIT_COMMAND} checkout -f $ENV{GIT_COMMIT})
endif()

######################################################
# Test custom update with a dashboard script.
message("Running CTest Dashboard Script (custom update)...")
include("${CTEST_SOURCE_DIRECTORY}/CTestConfig.cmake")
ctest_start(${MODEL})
ctest_update(SOURCE ${CTEST_SOURCE_DIRECTORY})
message("Updated.")
ctest_configure(SOURCE "${CTEST_SOURCE_DIRECTORY}" BUILD "${CTEST_BINARY_DIRECTORY}" APPEND)
message("Configured.")
ctest_submit(PARTS Update Configure Notes)

ctest_build(BUILD "${CTEST_BINARY_DIRECTORY}" APPEND)
message("Built.")
ctest_submit(PARTS Build)

ctest_test(BUILD "${CTEST_BINARY_DIRECTORY}" APPEND)
message("Tested.")
ctest_submit(PARTS Test)

message("DONE:CTestScript")
