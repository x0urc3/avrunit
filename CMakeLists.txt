cmake_minimum_required(VERSION 3.21)

include(FetchContent)
FetchContent_Declare(
    toolchain
    GIT_REPOSITORY https://github.com/x0urc3/avr-toolchain.cmake.git
    GIT_TAG 0c5db3feb7e5c3527524375fd78026161e5fbe85
    GIT_SHALLOW TRUE
    )
FetchContent_MakeAvailable(toolchain)
set(CMAKE_TOOLCHAIN_FILE ${toolchain_SOURCE_DIR}/avr-toolchain.cmake CACHE STRING "")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(avrunit)

project("avrunit")

enable_testing()
add_subdirectory(tests)
