|Criteria                    |Raspberri Pi SoC               |Arduino (Mega / UNO)                     |ESP32                                |
|:------:                    |:------:                       |:------:                                 |:------:                             |
| What's this?               | This is an System on Chip (SoC) | This is a Microcontroller (MCU)       | This is a Microcontroller (MCU)     |
| OS                         | General Purpose OS like Linux | Don't know                              | Real Time OS (e.g. FreeRTOS)        |
| Response times             | Having a full blown OS like Linux slows it down| ??                     | FreeRTOS works great                |
| IDE                        | Don't know                    | Arduino IDE, Eclipse, Scratch etc.      | Arduino IDE, Eclipse, VS Code etc.  |
| Pins                       | Don't know                    | Limited number                          | Limited number but supports i2c bus |
| Connectivity               | Supports Wifi/BT/BLE          | Wifi/BT/BLE need additional HW          | Built-in Wifi/BT/BLE                |
| Dev Framework              | Don't know                    | ??                                      | ESP-IDF is rich                     |
| Libraries                  | Don't know                    | Specific to peripheral devices          | Specific to peripheral devices      |
| Target group               | Intermediate to Advanced      | Beginner to intermediate                | Intermediate to Advanced            |
| Languages                  | MicroPython, C, C++           | MicroPython, C, C++                     | MicroPython, C, C++                 |
| Programming Approach       | MicroPython, C, C++           | setup() and loop() methods simply programming       | FreeRTOS supports Multitasking (threading), queues etc. ESP-IDF support interrupt handling |
