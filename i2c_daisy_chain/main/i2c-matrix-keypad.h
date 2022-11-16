// Good reference - https://embeddedexplorer.com/esp32-i2c-tutorial/

#include "driver/i2c.h"

xQueueHandle keyQueue; // used for storing what is read from PCF8574 keypad

void init_keypad(); // this will initialize data pins as well as interrupt pin
