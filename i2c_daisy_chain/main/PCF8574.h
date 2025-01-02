#include "hal/i2c_types.h"
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <esp_timer.h>

uint8_t read_byte_from_pins(i2c_port_t i2c_port_num); // read byte from pins at i2c port number

void write_byte_to_pins(i2c_port_t i2c_port_num, uint8_t data); // write byte to pins at i2c port number

extern esp_err_t err;
