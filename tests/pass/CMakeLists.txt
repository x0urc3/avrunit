add_executable(TEST_simple simple.c)
setup_avr_executable(TEST_simple)
add_avr_test(TEST_simple)
set_tests_properties(TEST_simple
    PROPERTIES PASS_REGULAR_EXPRESSION
    "P :1.*ID:0xd.*F :0.*ID:.*B :1.*ID:0xb.*I :1.*ID:0xa"
    )
