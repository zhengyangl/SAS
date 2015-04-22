set (CTEST_PROJECT_NAME "SAS")

macro(add_sas_test TEST_NAME TEST_SOURCE)
   set(SAS_TEST_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/test.py")
   set(SAS_CLANG "${CMAKE_BINARY_DIR}/scripts/clang")
   set(SAS_CLANGXX "${CMAKE_BINARY_DIR}/scripts/clang++")
   set(SAS_BINARY "${CMAKE_BINARY_DIR}/lib/libSas.so")

   # By default use the name of the test
   set(TEST_ENABLED_CHECKERS ${TEST_NAME})

   set(FULL_TEST_SOURCE "${CMAKE_SOURCE_DIR}/test/${TEST_SOURCE}")
   set(FULL_TEST_REF "${CMAKE_SOURCE_DIR}/test/${TEST_SOURCE}")
   string(REPLACE ".cpp" ".ref" FULL_TEST_REF ${FULL_TEST_SOURCE})

   add_test(NAME "${TEST_NAME}"
            COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${FULL_TEST_SOURCE}" "${SAS_BINARY}" "${TEST_ENABLED_CHECKERS}" "${FULL_TEST_REF}"
            WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/Testing")
endmacro(add_sas_test)

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

add_sas_test("sas.ThreadSafety.ConstCastAway" "ThreadSafety/const_cast_away.cpp")
add_sas_test("sas.ThreadSafety.ConstCast"     "ThreadSafety/const_cast.cpp")
add_sas_test("sas.ThreadSafety.GlobalStatic"  "ThreadSafety/global_static.cpp")
add_sas_test("sas.ThreadSafety.StaticLocal"   "ThreadSafety/static_local.cpp")
add_sas_test("sas.ThreadSafety.MutableMember" "ThreadSafety/mutable_member.cpp")

add_sas_test("sas.Performance.ArgSizeChecker"    "Performance/arg_size.cpp")
add_sas_test("sas.Performance.FiniteMathChecker" "Performance/finite_math.cpp")

add_sas_test("sas.Example.Varname"  "Example/varname.cpp")
