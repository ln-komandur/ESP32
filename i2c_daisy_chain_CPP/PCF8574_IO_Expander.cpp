/*
 * PCF8574_IO_Expander.cpp
 *
 *  Created on: Jan 4, 2025
 *      Author: Ln-Komandur
 */

#include <PCF8574_IO_Expander.hpp>

PCF8574_IO_Expander::PCF8574_IO_Expander()
{
	// Forced to have an empty  / default constructor
}

PCF8574_IO_Expander::PCF8574_IO_Expander(i2c_master_bus_handle_t bus_handle, uint8_t jumper_address)
{
	// Configure the device at a certain address
	i2c_device_config_t pcf8574_cfg = {};
	    pcf8574_cfg.dev_addr_length = I2C_ADDR_BIT_LEN_7,
	    pcf8574_cfg.device_address = jumper_address,
	    pcf8574_cfg.scl_speed_hz = 400000,
	    pcf8574_cfg.scl_wait_us = 5 * 1000, // BQ27441 can stretch up to 4ms, use 5ms to be safe.
	//};
	// Add the device to the master bus
	i2c_master_bus_add_device(bus_handle, &pcf8574_cfg, &device_handle);
}

void PCF8574_IO_Expander::write_byte_to_pins(uint8_t data)
{
	// This happens in the IO expander when writing data using it
	// Transmit / Write to the device
	int len = 1;
	uint8_t data_t[len];
	data_t[0] = data;
	i2c_master_transmit(device_handle, data_t, len, -1); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	// esp_rom_delay_us(100); // Per BQ27441 datasheet. - DONT KNOW HOW TO ADAPT IT TO PCF8574
	
}

uint8_t PCF8574_IO_Expander::read_byte_from_pins()
{
	// This happens in the IO expander when reading data using it
	uint8_t rx_data[1];
	// Receive / Read from the device
	i2c_master_receive(device_handle, rx_data, 1, -1); // -1 means wait forever.
	// ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);
	ESP_LOGI(TAG, "read_byte_from_pins");
	return rx_data[0]; // there is only one element in this array. Return it.
}
