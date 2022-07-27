# avrunit
This repository demonstrate unit testing for AVR software development. Test result could be stored in EEPROM or transmitted using USART.

# Getting Started
## Usage
1. Include a minimal header file ```avrunit.h``` in the test unit source code
2. By default, unit test results are stored in target AVR EEPROM. Declare ```AU_OUTPUT_SETUP()``` and redefine ```AU_OUTPUT()``` to redirect results to different output interface i.e. USART.

## Assertion Summary
Avrunit provides elementary assert test to reduce binary footprint size. The assert test available are

    AU_ASSERT(condition)

Evaluates whether condition is TRUE

    AU_IGNORED(condition)

Ignore the unit test. Useful if you want to exclude the unit test result.

    AU_BROKEN(condition)

Mark the unit test as broken.

# Example
## Unit test for avrunit
Unit testing for [avrunit.h](./avrunit.h) using CMake is included in [tests](./tests) folder.
Run the test as follows:

    $> cmake -S . -B build --toolchain ./cmake/avr-gnu.cmake
    $> cmake --build build --target test

# To Do
1. Create example that display result using USART
