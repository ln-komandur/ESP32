/*
 * PCF8574_IO_Expander.h
 *
 *  Created on: Jan 1, 2025
 * Functions for 
 * - reading from and writing to PCF8574 at device handle on an i2c bus at an i2c port
 * - getting the device handle
 * 
*/

#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <esp_timer.h>


// A handle for the device
extern i2c_master_dev_handle_t dev_handle;

i2c_master_dev_handle_t get_i2c_device_handle(i2c_master_bus_handle_t bus_handle, uint8_t device_address); // get the device handle

uint8_t read_byte_from_pins(i2c_master_dev_handle_t device_handle); // read byte from pins of the device handle

void write_byte_to_pins(i2c_master_dev_handle_t device_handle, uint8_t data); // write byte to pins of the device handle
