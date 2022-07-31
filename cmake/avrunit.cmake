set(AVRUNIT_RUNTEST_DEFAULT ON CACHE STRING "Store avrunit result in AVR EEPROM")
if (AVRUNIT_RUNTEST_DEFAULT)
    if (NOT Python)
        find_package(Python 3)
    endif ()

    if (Python_FOUND)
        set(AVRUNIT_DEFAULT_RUNTEST_CMD
            ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/avrunit-result.py
            )
    else()
        message(WARNING "Python3 unavailable. Inspect avrunit test result from EEPROM")
    endif()
endif()

set(AVRUNIT_RUNTEST_CMD ${AVRUNIT_DEFAULT_RUNTEST_CMD} CACHE STRING "Test command line")

function(add_avr_test FIRMWARE)
    if(NOT DEFINED AU_TEST_DELAY)
        set(AU_TEST_DELAY 2)
    endif()

    add_test(NAME AU_upload
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target upload_${FIRMWARE}
        )
    set_tests_properties(AU_upload PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)

    add_test(NAME AU_wait
        COMMAND ${CMAKE_COMMAND} -E sleep ${AU_TEST_DELAY}
        )
    set_tests_properties(AU_wait PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)

    if (AVRUNIT_RUNTEST_DEFAULT)
        add_test(NAME AU_get_result
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target dump_eeprom_${FIRMWARE}
            )
        set_tests_properties(AU_get_result PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)
    endif()

    add_test(NAME ${FIRMWARE}
        COMMAND ${AVRUNIT_RUNTEST_CMD} $<$<BOOL:AVRUNIT_RUNTEST_DEFAULT>:${FIRMWARE}.bin>
        )
    set_tests_properties( ${FIRMWARE} PROPERTIES FIXTURES_REQUIRED AU_TEST_FIXTURES)
endfunction()

function(set_avr_test_will_fail FIRMWARE)
    set_property(TEST ${FIRMWARE}
        PROPERTY WILL_FAIL TRUE
        )
endfunction()
