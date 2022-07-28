function(add_avr_test FIRMWARE)
    if(NOT DEFINED AU_TEST_DELAY)
        set(AU_TEST_DELAY 2)
    endif()

    add_test(NAME AU_upload
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target upload
        )
    set_tests_properties(AU_upload PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)

    add_test(NAME AU_wait
        COMMAND ${CMAKE_COMMAND} -E sleep ${AU_TEST_DELAY}
        )
    set_tests_properties(AU_wait PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)

    add_test(NAME AU_get_result
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR} --target dump_eeprom
        )
    set_tests_properties(AU_get_result PROPERTIES FIXTURES_SETUP AU_TEST_FIXTURES)


    if (NOT Python)
        find_package(Python 3)
    endif ()

    if (Python_FOUND)
        add_test(NAME ${FIRMWARE}
            COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/avrunit-result.py ${FIRMWARE}.bin)
        set_tests_properties( ${FIRMWARE} PROPERTIES FIXTURES_REQUIRED AU_TEST_FIXTURES)

    else()
        message(WARNING "Python3 unavailable. Inspect avrunit test result from EEPROM")
    endif()

endfunction()

function(set_avr_test_will_fail FIRMWARE)
    set_property(TEST ${FIRMWARE}
        PROPERTY WILL_FAIL TRUE
        )
endfunction()
