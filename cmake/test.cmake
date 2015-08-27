set (CTEST_PROJECT_NAME "SAS")

set(CTEST_SOURCE_DIRECTORY "$ENV{CMAKE_SOURCE_DIR}")
set(CTEST_BINARY_DIRECTORY "$ENV{CMAKE_BINARY_DIR}")
set(CTEST_COMMAND "ctest")

macro(add_sas_test_generic TEST_NAME TEST_SOURCE TEST_WORKING_DIRECTORY CONFIGURATION_YAML)
   set(SAS_TEST_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/test.py")
   set(SAS_CLANG "${CMAKE_BINARY_DIR}/scripts/clang")
   set(SAS_CLANGXX "${CMAKE_BINARY_DIR}/scripts/clang++")
   set(SAS_BINARY "${CMAKE_BINARY_DIR}/lib/libSas.so")

   # By default use the name of the test
   if(${TEST_NAME} MATCHES "^bwlist")
     set(TEST_ENABLED_CHECKERS "sas.CodingConventions.ROOT")
   elseif(${TEST_NAME} MATCHES "^commentDisabler")
     set(TEST_ENABLED_CHECKERS "sas.Example.Varname:sas.CodingConventions.General.NoUsingNamespaceInHeaders:sas.ThreadSafety.ConstCast")
   elseif(${TEST_NAME} MATCHES "^sas")
     set(TEST_ENABLED_CHECKERS ${TEST_NAME})
   else()
     set(TEST_ENABLED_CHECKERS "")
   endif()

   set(FULL_TEST_REF "${CMAKE_SOURCE_DIR}/test/${TEST_WORKING_DIRECTORY}${TEST_SOURCE}")
   string(REPLACE ".cpp" ".ref" FULL_TEST_REF ${FULL_TEST_REF})
   add_test(NAME "${TEST_NAME}"
     COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${TEST_SOURCE}" "${SAS_BINARY}" "${TEST_ENABLED_CHECKERS}" "${FULL_TEST_REF}" "${CONFIGURATION_YAML}"
     WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/test/${TEST_WORKING_DIRECTORY}")
 endmacro(add_sas_test_generic)

 macro(add_sas_test TEST_NAME TEST_SOURCE)
   add_sas_test_generic("${TEST_NAME}" "${TEST_SOURCE}" "" "")
 endmacro(add_sas_test)

macro(add_sas_test_wd TEST_NAME TEST_SOURCE TEST_WORKING_DIRECTORY)
   add_sas_test_generic("${TEST_NAME}" "${TEST_SOURCE}" "${TEST_WORKING_DIRECTORY}" "")
endmacro(add_sas_test_wd)

enable_testing()

add_sas_test("sas.CodingConventions.ROOT.RN3"                          "CodingConventions/ROOT/RN3.cpp")
add_sas_test("sas.CodingConventions.ROOT.RN4"                          "CodingConventions/ROOT/RN4.cpp")
add_sas_test("sas.CodingConventions.ROOT.RN6"                          "CodingConventions/ROOT/RN6.cpp")
add_sas_test("sas.CodingConventions.ROOT.RN9"                          "CodingConventions/ROOT/RN9.cpp")
add_sas_test("sas.CodingConventions.ROOT.RN10"                         "CodingConventions/ROOT/RN10.cpp")
add_sas_test("sas.CodingConventions.ROOT.RN11"                         "CodingConventions/ROOT/RN11.cpp")
add_sas_test("sas.CodingConventions.ROOT.RN12"                         "CodingConventions/ROOT/RN12.cpp")
add_sas_test("sas.CodingConventions.ROOT.PtrCastWin"                   "CodingConventions/ROOT/ptr_cast_win.cpp")
add_sas_test("sas.CodingConventions.General.NoUsingNamespaceInHeaders" "CodingConventions/General/using_namespace.cpp")
add_sas_test("sas.CodingConventions.General.StdPrintouts" "CodingConventions/General/std_printouts.cpp")

add_sas_test("sas.ThreadSafety.ConstCastAway" "ThreadSafety/const_cast_away.cpp")
add_sas_test("sas.ThreadSafety.ConstCast"     "ThreadSafety/const_cast.cpp")
add_sas_test("sas.ThreadSafety.GlobalStatic"  "ThreadSafety/global_static.cpp")
add_sas_test("sas.ThreadSafety.StaticLocal"   "ThreadSafety/static_local.cpp")
add_sas_test("sas.ThreadSafety.MutableMember" "ThreadSafety/mutable_member.cpp")

add_sas_test("sas.Performance.ArgSizeChecker"    "Performance/arg_size.cpp")
add_sas_test("sas.Performance.FiniteMathChecker" "Performance/finite_math.cpp")

add_sas_test_wd("bwlist.filepath.blacklist.match"  "BlackList/match.cpp" "BWList/FilePath/")
add_sas_test_wd("bwlist.filepath.blacklist.unmatch"  "BlackList/unmatch.cpp" "BWList/FilePath/")
add_sas_test_wd("bwlist.filepath.whitelist.match"  "WhiteList/match.cpp" "BWList/FilePath/")
add_sas_test_wd("bwlist.filepath.whitelist.unmatch"  "WhiteList/unmatch.cpp" "BWList/FilePath/")
add_sas_test_wd("bwlist.namespace.blacklist" "BlackList/match.cpp" "BWList/Namespace/")
add_sas_test_wd("bwlist.namespace.whitelist" "WhiteList/match.cpp" "BWList/Namespace/")
add_sas_test_wd("bwlist.class.blacklist" "BlackList/match.cpp" "BWList/Class/")
add_sas_test_wd("bwlist.class.whitelist" "WhiteList/match.cpp" "BWList/Class/")
add_sas_test_wd("bwlist.struct.blacklist" "BlackList/match.cpp" "BWList/Struct/")
add_sas_test_wd("bwlist.struct.whitelist" "WhiteList/match.cpp" "BWList/Struct/")
add_sas_test_generic("bwlist.locate_yaml_by_env" "match.cpp" "BWList/Misc/" "SA_CONFIGURATION=configuration.json")
add_sas_test("commentDisabler" "comment_checker_disabler.cpp")
add_sas_test_generic("clang-modernize" "loop_convert.cpp" "" "SA_MODERNIZE=-risk=safe -loop-convert")
add_sas_test("sas.Example.Varname"  "Example/varname.cpp")
