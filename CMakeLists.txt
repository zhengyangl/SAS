# Author: <CMS>
#         Filip Bartek (2013)

# Requirements:
# LLVM_DEV_DIR

# Usage:
# cmake -D LLVM_DEV_DIR=$LLVM_DEV_DIR $SAS_DIR

# To register a new checker class in the SAS library, add an argument
# "src/MyChecker.cpp" (replacing "MyChecker" with the actual name of the
# checker file) to the `add_clang_plugin` call further in this file.


cmake_minimum_required (VERSION 2.8)
project (sas CXX)

# Check if this is an out-of-source build
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
   message(FATAL_ERROR "In source build is not supported. Please build in a different directory.")
endif()

# Initialize input variables
set(LLVM_SRC_DIR "" CACHE FILEPATH "LLVM source directory")
set(LLVM_SRC_BUILD_DIR "" CACHE FILEPATH "LLVM build directory")

if (LLVM_DEV_DIR)
   set(LLVM_SRC_DIR ${LLVM_DEV_DIR})
   set(LLVM_SRC_BUILD_DIR ${LLVM_DEV_DIR})
else()
   message(FATAL_ERROR "You must set LLVM_DEV_DIR (`cmake -D LLVM_DEV_DIR=$LLVM_DEV_DIR`).")
endif()

# Require LLVM_SRC_DIR
if(NOT LLVM_SRC_DIR)
  message(FATAL_ERROR "You must set LLVM_SRC_DIR (`cmake -D LLVM_SRC_DIR=$LLVM_SRC_DIR`).")
endif()

# Require LLVM_SRC_BUILD_DIR
if(NOT LLVM_SRC_BUILD_DIR)
  message(FATAL_ERROR "You must set LLVM_SRC_BUILD_DIR (`cmake -D LLVM_SRC_BUILD_DIR=$LLVM_SRC_BUILD_DIR`).")
endif()
# /usr/lib/llvm-3.5/
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )


# the rest should work automatically

# assumes for clang to be under llvm/tools/, as suggested by the clang website 
# http://clang.llvm.org/get_started.html#build
set (CLANG_SRC_DIR  ${LLVM_SRC_DIR}/tools/clang)
set (CLANG_BUILD_DIR ${LLVM_SRC_BUILD_DIR}/tools/clang)

add_definitions (-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)
add_definitions (-D_GNU_SOURCE -DHAVE_CLANG_CONFIG_H)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-common -Woverloaded-virtual -Wcast-qual -fno-strict-aliasing -pedantic -Wno-long-long -Wall -W -Wno-unused-parameter -Wwrite-strings -fno-exceptions -fno-rtti -g")

set (CMAKE_MODULE_LINKER_FLAGS "-Wl,-flat_namespace -Wl,-undefined -Wl,suppress")

macro(add_clang_plugin name)
   set (srcs ${ARGN})

   include_directories( "${LLVM_SRC_DIR}/include"
      "${LLVM_SRC_BUILD_DIR}/include"
      "${CLANG_SRC_DIR}/include"
      "${CLANG_BUILD_DIR}/include" 
      "${CMAKE_CURRENT_SOURCE_DIR}/src")
   link_directories( "${LLVM_BUILD_DIR}/lib" )

   add_library( ${name} SHARED ${srcs} )

   if (SYMBOL_FILE)
      set_target_properties( ${name} PROPERTIES LINK_FlAGS
         "-exported_symbols_list ${SYMBOL_FILE}")
   endif()

   foreach (user_lib ${USER_LIBS})
      target_link_libraries( ${name} ${user_lib} )
   endforeach()

   set_target_properties( ${name} PROPERTIES
      LINKER_LANGUAGE CXX
      PREFIX "")
endmacro(add_clang_plugin)

set (USER_LIBS
   pthread
)

# Add new checkers to the plugin by adding their source files as arguments to
# the following call.
# The first argument is the plugin name.
add_clang_plugin(libSas
   src/SasException.cpp
   src/ConstCastChecker.cpp
   src/ConstCastAwayChecker.cpp
   src/StaticLocalChecker.cpp
   src/GlobalStaticChecker.cpp
   src/MutableMemberChecker.cpp
#    src/ClassDumper.cpp
   src/FiniteMathChecker.cc
   src/CatchAll.cpp
   src/UsingNamespace.cpp
   src/ArgSizeChecker.cpp
   src/VarnameChecker.cpp
   src/CheckerDisabler.cpp
   src/CodingConventions/ROOT/RN3Checker.cpp
   src/CodingConventions/ROOT/RN4Checker.cpp
   src/CodingConventions/ROOT/RN6Checker.cpp
   src/CodingConventions/ROOT/RN13Checker.cpp
   src/CodingConventions/ROOT/RN9FunctionsChecker.cpp
   src/CodingConventions/ROOT/PtrCastWinChecker.cpp
   src/ClangSasCheckerPluginRegister.cpp
#         src/GlobalAccInCtorChecker.cpp
        # Add your checker here!
)

file(COPY scripts USE_SOURCE_PERMISSIONS DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY config USE_SOURCE_PERMISSIONS DESTINATION ${CMAKE_BINARY_DIR}/)
if (TESTING)
file(COPY test DESTINATION ${CMAKE_BINARY_DIR}/)
endif()


