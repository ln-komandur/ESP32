// Good reference - https://embeddedexplorer.com/esp32-i2c-tutorial/

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <driver/i2c.h>

extern esp_err_t err;

extern QueueHandle_t keyQueue; // used for storing what is read from PCF8574 keypad

void init_keypad(i2c_port_t i2c_port_num); // this will assign the i2c port, and initialize data pins as well as interrupt pin
