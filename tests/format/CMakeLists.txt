file(COPY pass.bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME TEST_runtest_script_pass
    COMMAND ${AVRUNIT_DEFAULT_RUNTEST_CMD} -f pass.bin
    )
set_tests_properties(TEST_runtest_script_pass
    PROPERTIES PASS_REGULAR_EXPRESSION
    "P :1.*ID:0xd[^,]*F :0.*ID:[^0]*B :1.*ID:0xb[^,]*I :1.*ID:0xa[^,]*"
    )

file(COPY fail.bin DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_test(NAME TEST_runtest_script_fail
    COMMAND ${AVRUNIT_DEFAULT_RUNTEST_CMD} -f fail.bin
    )
set_tests_properties(TEST_runtest_script_fail
    PROPERTIES PASS_REGULAR_EXPRESSION
    "P :0.*ID:[^0]*F :1.*ID:0xc[^,]*B :0.*ID:[^0]*I :0.*ID:[^0]*"
    )
