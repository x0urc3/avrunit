cmake_minimum_required(VERSION 3.21)

include(FetchContent)
FetchContent_Declare(
    toolchain
    GIT_REPOSITORY https://github.com/x0urc3/avr-toolchain.cmake.git
    GIT_TAG e41c890d62f03f7da6bcb5da064d3bfb4ba9edf9
    GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
    GIT_SHALLOW TRUE
    )
FetchContent_MakeAvailable(toolchain)
set(CMAKE_TOOLCHAIN_FILE ${toolchain_SOURCE_DIR}/avr-toolchain.cmake CACHE STRING "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(avrunit)

project("avrunit")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    enable_testing()
    add_subdirectory(tests)
endif()
