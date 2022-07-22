cmake_minimum_required(VERSION 3.21)

project("example")
add_executable(firmware main.c led.c)
setup_avr_executable(firmware)
