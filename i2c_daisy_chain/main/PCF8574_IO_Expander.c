/*
 * PCF8574_IO_Expander.c
 *
 *  Created on: Jan 1, 2025
 * Functions for 
 * - reading from and writing to PCF8574 at device handle on an i2c bus at an i2c port
 * - getting the device handle
 * 
*/

#include <PCF8574_IO_Expander.h>
#include <stdint.h>

static const char *TAG = "PCF8574_IO_Expander";

i2c_master_dev_handle_t get_i2c_device_handle(i2c_master_bus_handle_t bus_handle, uint8_t dev_address)
{
	i2c_master_dev_handle_t device_handle;

	// Configure the device at a certain address
	i2c_device_config_t pcf8574_cfg = {
	    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
	    .device_address = dev_address,
	    .scl_speed_hz = 400000,
	    .scl_wait_us = 5 * 1000, // BQ27441 can stretch up to 4ms, use 5ms to be safe. From https://github.com/espressif/esp-idf/issues/14401
	};
	// Add the device to the master bus
	i2c_master_bus_add_device(bus_handle, &pcf8574_cfg, &device_handle);
	ESP_LOGI(TAG, "get_i2c_device_handle: Got a Device Handle: About to return that DEVICE handle");
	return device_handle; // Return the device handle
}

uint8_t read_byte_from_pins(i2c_master_dev_handle_t device_handle)
{
	uint8_t rx_data[1];
	// Receive / Read from the device
	i2c_master_receive(device_handle, rx_data, 1, -1); // -1 means wait forever.
	ESP_LOGI(TAG, "read byte from PCF8574 pins at its device handle. About to return that byte");
	return rx_data[0]; // there is only one element in this array. Return it.
}

void write_byte_to_pins(i2c_master_dev_handle_t device_handle, uint8_t data)
{
	// Transmit / Write to the device
	int len = 1;
	uint8_t data_t[len];
	data_t[0] = data;
	i2c_master_transmit(device_handle, data_t, len, -1); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	// esp_rom_delay_us(100); // Per BQ27441 datasheet. - DONT KNOW HOW TO ADAPT IT TO PCF8574 from https://github.com/espressif/esp-idf/issues/14401
	//ESP_LOGI(TAG, "wrote byte to PCF8574 pins at its device handle. About to return");
}
