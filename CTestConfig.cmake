set (CTEST_PROJECT_NAME "SAS")

macro(add_sas_test TEST_NAME TEST_SOURCE TEST_ENABLED_CHECKERS TEST_EXPECT_OUTPUT)
   set(SAS_TEST_SCRIPT "${CMAKE_SOURCE_DIR}/scripts/test.py")
   set(SAS_CLANG "${CMAKE_BINARY_DIR}/scripts/clang")
   set(SAS_CLANGXX "${CMAKE_BINARY_DIR}/scripts/clang++")
   set(SAS_BINARY "${CMAKE_BINARY_DIR}/lib/libSas.so")

   add_test(NAME "${TEST_NAME}" COMMAND "python" "${SAS_TEST_SCRIPT}" "${SAS_CLANGXX}" "${CMAKE_SOURCE_DIR}/test/${TEST_SOURCE}" "${SAS_BINARY}" "${TEST_ENABLED_CHECKERS}" "${TEST_EXPECT_OUTPUT}")
endmacro(add_sas_test)

enable_testing()
add_sas_test("sas.CodingConventions.ROOT.RN3"  "CodingConventions/ROOT/RN3.cpp"  "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN3]")
add_sas_test("sas.CodingConventions.ROOT.RN4"  "CodingConventions/ROOT/RN4.cpp"  "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN4]")
add_sas_test("sas.CodingConventions.ROOT.RN6"  "CodingConventions/ROOT/RN6.cpp"  "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN6]")
add_sas_test("sas.CodingConventions.ROOT.RN9"  "CodingConventions/ROOT/RN9.cpp"  "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN9]")
add_sas_test("sas.CodingConventions.ROOT.RN10" "CodingConventions/ROOT/RN10.cpp" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN10]")
add_sas_test("sas.CodingConventions.ROOT.RN11" "CodingConventions/ROOT/RN11.cpp" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN11]")
add_sas_test("sas.CodingConventions.ROOT.RN12" "CodingConventions/ROOT/RN12.cpp" "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.RN12]")
add_sas_test("sas.CodingConventions.ROOT.PtrCastWinChecker"  "CodingConventions/ROOT/ptr_cast_win.cpp"  "sas.CodingConventions.ROOT" "[sas.CodingConventions.ROOT.PtrCastWinChecker]")
add_sas_test("sas.CodingConventions.General.NoUsingNamespaceInHeaders" "CodingConventions/General/using_namespace.cpp"  "sas.CodingConventions.General" "[sas.CodingConventions.General.NoUsingNamespaceInHeaders]")
add_sas_test("sas.ThreadSafety.ConstCastAwayChecker" "ThreadSafety/const_cast_away.cpp" "sas.ThreadSafety" "[sas.ThreadSafety.ConstCastAwayChecker]")
add_sas_test("sas.ThreadSafety.ConstCastChecker"  "ThreadSafety/const_cast.cpp"  "sas.ThreadSafety" "[sas.ThreadSafety.ConstCastChecker]")
add_sas_test("sas.ThreadSafety.GlobalStaticChecker" "ThreadSafety/global_static.cpp" "sas.ThreadSafety" "[sas.ThreadSafety.GlobalStaticChecker]")
add_sas_test("sas.ThreadSafety.StaticLocalChecker" "ThreadSafety/static_local.cpp" "sas.ThreadSafety" "[sas.ThreadSafety.StaticLocalChecker]")
add_sas_test("sas.ThreadSafety.MutableMemberChecker" "ThreadSafety/mutable_member.cpp" "sas.ThreadSafety" "[sas.ThreadSafety.MutableMemberChecker]")
add_sas_test("sas.Performance.ArgSizeChecker"  "Performance/arg_size.cpp" "sas.Performance" "[sas.Performance.ArgSizeChecker]")
add_sas_test("sas.Performance.FiniteMathChecker" "Performance/finite_math.cpp" "sas.Performance" "[sas.Performance.FiniteMathChecker]")
add_sas_test("sas.Example.Varname"  "Example/varname.cpp" "sas.Example" "[sas.Example.Varname]")
