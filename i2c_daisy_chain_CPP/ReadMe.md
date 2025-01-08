# What's in the CPP Code?

## Current capabilities
The C++ version of i2c daisy chaining currently only 
- read the addresses of all the devices connected to the i2c master bus and prints them
- blink a string of 8 LEDs connected to one PCF8574 expander
  - This uses an autonomous task (not using any queues or interrupts)

## Underlining frameworks and drivers
The source files are on built on
- `FreeRTOS v8.0.0`
- `esp-idf v5.3.1`
- `<driver/i2c_master.h>` instead of the legacy `<driver/i2c.h>` 
