# avr-gnu.cmake Copyright (c) 2022 Khairulmizam Samsudin <xource@gmail.com
#
# CMake toolchain for AVR microcontroller that depends on GNU AVR project and avrdude
#

set(AVR_BOARD_TYPE "None" CACHE STRING "Set pre-configured AVR boards")
set_property(CACHE AVR_BOARD_TYPE PROPERTY STRINGS "None" "Arduino")

set(AVR_MCU_F 1000000UL CACHE STRING "Set -DF_CPU")
set(AVR_MCU atmega328p CACHE STRING "Set -mmcu")

set(TOOL_UPLOAD_PORT /dev/ttyUSB0 CACHE STRING "Set tool serial port")
set(TOOL_UPLOAD_ARGS -c stk500v1 -P ${TOOL_UPLOAD_PORT} -b 19200 -q
    CACHE STRING "Set avrdude arguments")

if (AVR_BOARD_TYPE STREQUAL "Arduino")
    set(AVR_MCU_F 16000000UL CACHE STRING "Set -DF_CPU" FORCE)
    set(AVR_MCU atmega328p CACHE STRING "Set -mmcu" FORCE)
    set(TOOL_UPLOAD_ARGS -c arduino -P ${TOOL_UPLOAD_PORT} -b 115200 -q
        CACHE STRING "Set avrdude arguments: Default: -c stk500v1 -P /dev/ttyUSB0 -b 19200 -q"
        FORCE)
    set(AVR_BOARD_ARDUINO TRUE CACHE STRING "Using Arduino compatible board")
else()
    set(TOOL_UPLOAD_ARGS -c stk500v1 -P /dev/ttyUSB0 -b 19200 -q
        CACHE STRING "Set avrdude arguments"
        FORCE)
    unset(AVR_BOARD_ARDUINO CACHE)
endif ()

message(STATUS "Building for AVR ${AVR_MCU} with board=${AVR_BOARD_TYPE}")

set(TOOL_SIZE_ARGS -C CACHE STRING "Set arguments. Default: -C")
set(AVR_BAUD 9600 CACHE STRING "Set AVR Baudrate. Default: 9600")
set(AVR_FUSE_L 0x62 CACHE STRING "Set Low Fuse. Default: 0x62")
set(AVR_FUSE_H 0xd6 CACHE STRING "Set High Fuse. Default: 0xd6")
set(AVR_FUSE_E 0xfd CACHE STRING "Set Extended Fuse. Default: 0xfd")
set(AVR_LOCKBIT 0xfd CACHE STRING "Set Lock Bit. Default: 0xff")

set(CMAKE_SYSTEM_NAME Generic)
find_program(CMAKE_C_COMPILER avr-gcc REQUIRED)
#if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 7.3)
#    message(FATAL_ERROR "GCC version must be at least 7.3!")
#endif()
find_program(CMAKE_CXX_COMPILER avr-g++ REQUIRED)
find_program(CMAKE_AR avr-gcc-ar REQUIRED)
find_program(CMAKE_RANLIB avr-gcc-ranlib REQUIRED)
find_program(TOOL_UPLOAD avrdude REQUIRED DOC "Set AVR upload tool. Default: avrdude")
find_program(TOOL_SIZE avr-size REQUIRED DOC "Set binary size tool. Default: avr-size")
find_program(TOOL_STRIP avr-strip REQUIRED DOC "Set binary strip tool. Default: avr-strip")

function(add_avr_custom_target FIRMWARE)

    add_custom_target(upload_eeprom ${CMAKE_OBJCOPY} -j .eeprom  --set-section-flags=.eeprom="alloc,load" --change-section-lma .eeprom=0 -O ihex ${FIRMWARE}.elf ${FIRMWARE}.eep
        COMMAND ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U eeprom:w:${FIRMWARE}.eep
        DEPENDS ${FIRMWARE}
        )

    add_custom_target(fuses ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U lfuse:w:${AVR_FUSE_L}:m -U hfuse:w:${AVR_FUSE_H}:m -U efuse:w:${AVR_FUSE_E}:m -U lock:w:${AVR_LOCKBIT}:m
        )

    add_custom_target(show_fuses ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h -U lock:r:-:h
        )

    add_custom_target(avrdude_terminal ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -t
        )

endfunction()

function(setup_avr_target FIRMWARE)
    target_compile_definitions(${FIRMWARE} PRIVATE
        -DF_CPU=${AVR_MCU_F}
        -DBAUD=${AVR_BAUD}
        )

    target_link_options(${FIRMWARE} PRIVATE
        -mmcu=${AVR_MCU}
        -flto
        )

    target_compile_options(${FIRMWARE} PRIVATE
        -mmcu=${AVR_MCU} # AVR_MCU
        -Os
        -std=gnu11
        -Wall # enable warnings
        -Wstrict-prototypes
        -Wfatal-errors
        -Wl,--gc-sections
        -flto
        -fuse-linker-plugin
        )

    get_target_property(target_type ${FIRMWARE} TYPE)
    if (target_type STREQUAL "EXECUTABLE")

        set_target_properties(${FIRMWARE} PROPERTIES OUTPUT_NAME ${FIRMWARE}.elf)

        add_custom_command(TARGET ${FIRMWARE} POST_BUILD
            COMMAND "${TOOL_SIZE};${TOOL_SIZE_ARGS};${FIRMWARE}.elf"
            COMMAND "$<$<CONFIG:release,minsizerel>:${TOOL_STRIP};${FIRMWARE}.elf>"
            COMMAND "${CMAKE_OBJCOPY};-R .eeprom -O ihex;${FIRMWARE}.elf;${FIRMWARE}.hex"
            COMMAND_EXPAND_LISTS
            )

        if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
            set(NAME_UPLOAD upload)
            set(NAME_DUMP_EEPROM dump_eeprom)
        else()
            set(NAME_UPLOAD upload_${FIRMWARE})
            set(NAME_DUMP_EEPROM dump_eeprom_${FIRMWARE})
        endif()

        add_custom_target(${NAME_UPLOAD}
            ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U flash:w:${FIRMWARE}.hex
            DEPENDS ${FIRMWARE}
            )

        add_custom_target(${NAME_DUMP_EEPROM}
            ${TOOL_UPLOAD} ${TOOL_UPLOAD_ARGS} -p ${AVR_MCU} -U eeprom:r:${FIRMWARE}.bin:r
            )

        if(CMAKE_BINARY_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)
            add_avr_custom_target(${FIRMWARE})
        endif()

        set_directory_properties(
            PROPERTIES ADDITIONAL_CLEAN_FILES "${FIRMWARE}.hex;${FIRMWARE}.eep;${FIRMWARE}.bin"
            )
    endif()

endfunction()
