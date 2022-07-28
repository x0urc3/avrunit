add_executable(TEST_simple simple.c)
setup_avr_executable(TEST_simple)
add_avr_test(TEST_simple)
