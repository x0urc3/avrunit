cmake_minimum_required(VERSION 3.21)

include(FetchContent)
FetchContent_Declare(
    toolchain
    GIT_REPOSITORY https://github.com/x0urc3/avr-toolchain.cmake.git
    GIT_TAG 5135f332f01c1ef4b603ce8b948ead781b8e66c5
    GIT_SHALLOW TRUE
    )
FetchContent_MakeAvailable(toolchain)
set(CMAKE_TOOLCHAIN_FILE ${toolchain_SOURCE_DIR}/avr-toolchain.cmake CACHE STRING "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(avrunit)

project("avrunit")

enable_testing()
add_subdirectory(tests)
