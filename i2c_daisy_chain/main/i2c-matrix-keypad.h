// Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/


#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"



uint8_t read_keypad_pins(void);

void write_to_keypad(uint8_t data);

