function(add_avr_test FIRMWARE)
    if(NOT DEFINED AU_TEST_DELAY)
        set(AU_TEST_DELAY 2)
    endif()

    add_custom_command(TARGET ${FIRMWARE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E sleep ${AU_TEST_DELAY}
        COMMAND ${CMAKE_COMMAND} -E echo Waiting for test to complete
        COMMAND ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U eeprom:r:${FIRMWARE}.bin:r
        )

    if (NOT Python)
        find_package(Python 3)
    endif ()

    if (Python_FOUND)
        add_test(NAME ${FIRMWARE}
            COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/scripts/avrunit-result.py ${FIRMWARE}.bin)

    else()
        message(WARNING "Python3 unavailable. Manually build avrunit test and dump EEPROM")
    endif()

endfunction()

function(set_avr_test_will_fail FIRMWARE)
    set_property(TEST AU_T_${FIRMWARE}
        PROPERTY WILL_FAIL TRUE
        )
endfunction()
