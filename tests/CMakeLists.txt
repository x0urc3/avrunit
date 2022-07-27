add_executable(simple simple.c)
setup_avr_executable(simple)
add_avr_test(simple)
set_avr_test_will_fail(simple)

