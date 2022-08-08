add_executable(TEST_fail simple.c)
target_link_libraries(TEST_fail avrunit)
setup_avr_target(TEST_fail)

add_avr_test(TEST_fail)
set_tests_properties(TEST_fail
    PROPERTIES PASS_REGULAR_EXPRESSION
    "P :0.*ID:.*F :1.*ID:0xc.*B :0.*ID:.*I :0.*ID:"
    )
