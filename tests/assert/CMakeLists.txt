add_executable(TEST_assert assert.c)
target_link_libraries(TEST_assert avrunit)
setup_avr_target(TEST_assert)

add_avr_test(TEST_assert)
