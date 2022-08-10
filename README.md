# avrunit
A unit testing framework for AVR software development.

## Feature
1. A minimalist test framework i.e SLOC < 120 including USART support
2. Test result could be stored in EEPROM (default) or sent to host using USART.
3. Tested with GCC and CMake with [avr-toolchain.cmake](https://github.com/x0urc3/avr-toolchain.cmake)

# Getting Started
## Simple Usage
1. Include a header file ```avrunit.h``` in the test unit source code
2. Declare ```AU_OUTPUT_SETUP()``` and redefine ```AU_OUTPUT()``` to redirect results to different output interface i.e. USART.
3. Define AU_SERIAL macro (i.e. -DAU_SERIAL) to enable unittest result dump through USART 

## Usage with cmake
1. Include the following files
   * [cmake/avr-toolchain.cmake](./cmake/avr-toolchain.cmake)
   * [cmake/avrunit.cmake](./cmake/avrunit.cmake)
   * [scripts/avrunit-result.py](./scripts/avrunit-result.py)
2. Build, configure and upload. See avrunit own unit test.

## Assertion Summary
avrunit provides elementary assert test to reduce binary footprint size. The assert test available are

    AU_ASSERT(condition)

Evaluates whether condition is TRUE

    AU_IGNORED(condition)

Ignore the unit test. Useful if you want to exclude the unit test result.

    AU_BROKEN(condition)

Mark the unit test as broken.

## Unit test for avrunit
Unit testing for [avrunit.h](./avrunit.h) using CMake is included in [tests](./tests) folder.
Run the test as follows:

    cmake -S . -B build
    cmake --build build
    cmake --build build --target test
