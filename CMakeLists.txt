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
set(LLVM_INCLUDE_DIR "" CACHE FILEPATH "LLVM include directory")
set(LLVM_LIB_DIR "" CACHE FILEPATH "LLVM lib directory")
set(CLANG_INCLUDE_DIRS "" CACHE FILEPATH "Clang include directory")

if (LLVM_DEV_DIR)
   message(STATUS "Manually set LLVM_DEV_DIR: ${LLVM_DEV_DIR}")
   set(LLVM_INLCUDE_DIR "${LLVM_DEV_DIR}/include")
   set(LLVM_LIB_DIR "${LLVM_DEV_DIR}/lib")
   set(CLANG_INCLUDE_DIRS "${LLVM_DEV_DIR}/include")
else()
   INCLUDE(cmake/FindLLVM.cmake)
   INCLUDE(cmake/FindClang.cmake)

   # Require LLVM_INCLUDE_DIR
   if (LLVM_INCLUDE_DIR)
      set(LLVM_INCLUDE_DIR ${LLVM_INCLUDE_DIR})
   else()
      message(FATAL_ERROR "Could not find LLVM headers. Install LLVM headers please")
   endif()

   # Require LLVM_LIB_DIR
   if(LLVM_LIB_DIR)
   else()
      message(FATAL_ERROR "Could not find LLVM lib. Install LLVM lib please")
   endif()

   # Require CLANG_FOUND
   if(NOT CLANG_FOUND)
      message(FATAL_ERROR "Could not find Clang headers. Install Clang please")
   endif()
endif()
# /usr/lib/llvm-3.5/
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )


# the rest should work automatically

# assumes for clang to be under llvm/tools/, as suggested by the clang website
# http://clang.llvm.org/get_started.html#build

add_definitions (-D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS)
add_definitions (-D_GNU_SOURCE -DHAVE_CLANG_CONFIG_H)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fno-common -Woverloaded-virtual -Wcast-qual -fno-strict-aliasing -pedantic -Wno-long-long -Wall -W -Wno-unused-parameter -Wwrite-strings -fno-exceptions -fno-rtti -O3")

set (CMAKE_MODULE_LINKER_FLAGS "-Wl,-flat_namespace -Wl,-undefined -Wl,suppress")

macro(add_clang_plugin name)
   set (srcs ${ARGN})

   include_directories(
      "${LLVM_INLCUDE_DIR}/"
      "${CLANG_INCLUDE_DIRS}"
      "${CMAKE_CURRENT_SOURCE_DIR}/src")
   link_directories( "${LLVM_LIB_DIR}/" )

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
#    src/ClassDumper.cpp
   src/CatchAll.cpp
   src/VarnameChecker.cpp
   src/CheckerDisabler.cpp
   # New Structure
   src/CodingConventions/General/UsingNamespace.cpp
   src/CodingConventions/ROOT/RN3Checker.cpp
   src/CodingConventions/ROOT/RN4Checker.cpp
   src/CodingConventions/ROOT/RN6Checker.cpp
   src/CodingConventions/ROOT/RN9Checker.cpp
   src/CodingConventions/ROOT/RN10Checker.cpp
   src/CodingConventions/ROOT/RN11Checker.cpp
   src/CodingConventions/ROOT/RN12Checker.cpp
   src/CodingConventions/ROOT/RN13Checker.cpp
   src/CodingConventions/ROOT/PtrCastWinChecker.cpp
   src/ThreadSafety/ConstCastChecker.cpp
   src/ThreadSafety/ConstCastAwayChecker.cpp
   src/ThreadSafety/GlobalStaticChecker.cpp
   src/ThreadSafety/StaticLocalChecker.cpp
   src/ThreadSafety/MutableMemberChecker.cpp
   src/Performance/ArgSizeChecker.cpp
   src/Performance/FiniteMathChecker.cpp
   src/ClangSasCheckerPluginRegister.cpp
#         src/GlobalAccInCtorChecker.cpp
        # Add your checker here!
)

file(COPY scripts USE_SOURCE_PERMISSIONS DESTINATION ${CMAKE_BINARY_DIR}/)
file(COPY config USE_SOURCE_PERMISSIONS DESTINATION ${CMAKE_BINARY_DIR}/)
if (TESTING)
file(COPY test DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

# CTEST BASED TESTING

set(SAS_TEST_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/test.py")
set(SAS_CLANG "${CMAKE_BINARY_DIR}/scripts/clang")
set(SAS_CLANGXX "${CMAKE_BINARY_DIR}/scripts/clang++")
set(SAS_BINARY "${CMAKE_BINARY_DIR}/lib/libSas.so")
enable_testing()
add_test(NAME "sas.CodingConventions.ROOT.RN3"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN3.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN3]")
add_test(NAME "sas.CodingConventions.ROOT.RN4"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN4.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN4]")
add_test(NAME "sas.CodingConventions.ROOT.RN6"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN6.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN6]")
add_test(NAME "sas.CodingConventions.ROOT.RN9"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN9.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN9]")
add_test(NAME "sas.CodingConventions.ROOT.RN10"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN10.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN10]")
add_test(NAME "sas.CodingConventions.ROOT.RN11"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN11.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN11]")
add_test(NAME "sas.CodingConventions.ROOT.RN12"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/RN12.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN12]")
add_test(NAME "sas.CodingConventions.ROOT.PtrCastWinChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/ROOT/ptr_cast_win.cpp" "${SAS_BINARY}" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.PtrCastWinChecker]")
add_test(NAME "sas.CodingConventions.General.NoUsingNamespaceInHeaders"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/CodingConventions/General/using_namespace.cpp" "${SAS_BINARY}" "sas.CodingConventions.General" "[sas.CodingConventions.General.NoUsingNamespaceInHeaders]")
add_test(NAME "sas.ThreadSafety.ConstCastAwayChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/ThreadSafety/const_cast_away.cpp" "${SAS_BINARY}" "sas.ThreadSafety" "[sas.ThreadSafety.ConstCastAwayChecker]")
add_test(NAME "sas.ThreadSafety.ConstCastChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/ThreadSafety/const_cast.cpp" "${SAS_BINARY}" "sas.ThreadSafety" "[sas.ThreadSafety.ConstCastChecker]")
add_test(NAME "sas.ThreadSafety.GlobalStaticChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/ThreadSafety/global_static.cpp" "${SAS_BINARY}" "sas.ThreadSafety" "[sas.ThreadSafety.GlobalStaticChecker]")
add_test(NAME "sas.ThreadSafety.StaticLocalChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/ThreadSafety/static_local.cpp" "${SAS_BINARY}" "sas.ThreadSafety" "[sas.ThreadSafety.StaticLocalChecker]")
add_test(NAME "sas.ThreadSafety.MutableMemberChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/ThreadSafety/mutable_member.cpp" "${SAS_BINARY}" "sas.ThreadSafety" "[sas.ThreadSafety.MutableMemberChecker]")
add_test(NAME "sas.Performance.ArgSizeChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/Performance/arg_size.cpp" "${SAS_BINARY}" "sas.Performance" "[sas.Performance.ArgSizeChecker]")
add_test(NAME "sas.Performance.FiniteMathChecker"  COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/Performance/finite_math.cpp" "${SAS_BINARY}" "sas.Performance" "[sas.Performance.FiniteMathChecker]")