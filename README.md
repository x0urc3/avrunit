# avrunit
This repository demonstrate unit testing for AVR software development. 

# Usage
1. Include a minimal header file ```avrunit.h``` in the tesh unit source code
2. Declare function ```au_output_setup()``` and ```au_output()``` to display the results. Tests results can printed using USART or stored in EEPROM/FLASH memory. 

# Example
This repository demonstrate a simple example for ATmega48 using [PlatformIO](https://platformio.org/). **NOTE**: This example does **NOT** use PlatformIO test feature. 

To compile and upload program
```console 
$ pio run -e dev -t upload 
```
To run unit test and print result using USART. **NOTE** Baud rate is 9600
```console 
$ pio run -e test -t upload 
```
