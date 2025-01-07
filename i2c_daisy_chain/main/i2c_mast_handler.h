/*
 * i2c_mast_handler.h
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 *
 * Manages (methods to initialize, check) i2c ports for ESP32
 * Reference - https://embeddedexplorer.com/esp32-i2c-tutorial/ - The code here is originally from https://gist.github.com/herzig/8d4c13d8b81a77ac86481c6c1306bb12
 */


#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <freertos/FreeRTOS.h>

#define MAX_NO_OF_I2C_DEVICES 128 // This is the maximum number of I2C devices that can be connected


/*
 * Initializes an i2c bus at the given i2c port number and returns the bus handle
 */
i2c_master_bus_handle_t get_i2c_bus_handle(int i2c_num);


/*
 * Returns a pointer to an array of i2c_device_addresses for the given bus handle
 */
uint8_t* get_connected_i2c_device_addresses(i2c_master_bus_handle_t bus_handle);
