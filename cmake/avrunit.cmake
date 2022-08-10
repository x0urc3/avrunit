if (AVR_BOARD_ARDUINO)
    set(AVRUNIT_RUNTEST_OUTPUT "Serial" CACHE STRING "Set result output" FORCE)
    message(STATUS "AVRUNIT_RUNTEST_OUTPUT=Serial since Arduino does not support EEPROM dump")
else()
    set(AVRUNIT_RUNTEST_OUTPUT "EEPROM" CACHE STRING "Set result output" FORCE)
endif()
set_property(CACHE AVRUNIT_RUNTEST_OUTPUT PROPERTY STRINGS "EEPROM" "Serial" "Custom")

if (NOT AVRUNIT_RUNTEST_OUTPUT STREQUAL "Custom")
    if (NOT Python)
        find_package(Python 3)
    endif ()

    if (Python_FOUND)
        set(AVRUNIT_DEFAULT_RUNTEST_CMD
            ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/avrunit-result.py
            CACHE STRING "Default test command")
    else()
        message(WARNING "Python3 unavailable. Inspect test result from ${AVRUNIT_RUNTEST_OUTPUT}")
    endif()
endif()

set(AVRUNIT_RUNTEST_CMD ${AVRUNIT_DEFAULT_RUNTEST_CMD} CACHE STRING "Test command")
if(NOT AVRUNIT_DEFAULT_RUNTEST_CMD)
    message(WARNING "Set test command in AVRUNIT_RUNTEST_CMD")
endif()

function(set_avr_test_delay DELAY)
    set(AU_TEST_DELAY ${DELAY})
endfunction()

function(add_avr_test FIRMWARE)
    if (AVR_BOARD_ARDUINO)
        target_compile_definitions(${FIRMWARE} PRIVATE
            -DAU_SERIAL
            )
    endif()

    add_test(NAME AU_upload
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target upload_${FIRMWARE}
        )
    set_tests_properties(AU_upload PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)

    if(DEFINED AU_TEST_DELAY)
        add_test(NAME AU_wait
            COMMAND ${CMAKE_COMMAND} -E sleep ${AU_TEST_DELAY}
            )
        set_tests_properties(AU_wait PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)
    endif()

    if (AVRUNIT_RUNTEST_OUTPUT STREQUAL "EEPROM")
        add_test(NAME AU_get_result
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target dump_eeprom_${FIRMWARE}
            )
        set_tests_properties(AU_get_result PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)
        add_test(NAME ${FIRMWARE}
            COMMAND ${AVRUNIT_RUNTEST_CMD} -f ${FIRMWARE}.bin
            )
        set_tests_properties( ${FIRMWARE} PROPERTIES FIXTURES_REQUIRED AU_TEST_FIXTURES)
    endif()

    if (AVRUNIT_RUNTEST_OUTPUT STREQUAL "Serial")
        add_test(NAME ${FIRMWARE}
            COMMAND ${AVRUNIT_RUNTEST_CMD} -p ${TOOL_UPLOAD_PORT} -b ${AVR_BAUD}
            )
        set_tests_properties( ${FIRMWARE} PROPERTIES FIXTURES_REQUIRED AU_TEST_FIXTURES)
    endif()

    if (AVRUNIT_RUNTEST_OUTPUT STREQUAL "Custom")
        add_test(NAME ${FIRMWARE}
            COMMAND ${AVRUNIT_RUNTEST_CMD}
            )
        set_tests_properties( ${FIRMWARE} PROPERTIES FIXTURES_REQUIRED AU_TEST_FIXTURES)
    endif()

endfunction()

function(set_avr_test_will_fail FIRMWARE)
    set_property(TEST ${FIRMWARE}
        PROPERTY WILL_FAIL TRUE
        )
endfunction()
