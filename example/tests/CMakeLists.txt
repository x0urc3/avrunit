add_executable(test_simple simple.c)

set(FIRMWARE test_simple)
setup_avr_executable(${FIRMWARE})
set_property(TARGET ${FIRMWARE}
    PROPERTY EXCLUDE_FROM_ALL)
add_custom_command(TARGET ${FIRMWARE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -R .eeprom -O ihex ${FIRMWARE}.elf ${FIRMWARE}.hex
    COMMAND ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U flash:w:${FIRMWARE}.hex
    )

if(NOT DEFINED AU_TEST_DELAY)
    set(AU_TEST_DELAY 2)
endif()

add_custom_command(TARGET ${FIRMWARE} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E sleep ${AU_TEST_DELAY}
    COMMAND ${CMAKE_COMMAND} -E echo Waiting for test to complete
    COMMAND ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U eeprom:r:${FIRMWARE}.bin:r
    )

#execute_process(COMMAND ${CMAKE_COMMAND} -E echo test)
#add_test(NAME TestSimple COMMAND tests)
