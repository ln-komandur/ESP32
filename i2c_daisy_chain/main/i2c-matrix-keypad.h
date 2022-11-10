// Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/


#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"



uint8_t get_keypad_pins(void);

void set_keypad_pins(uint8_t data);

char find_key(void);
