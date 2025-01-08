/*
 * PCF8574_IO_Expander.hpp
 *
 *  Created on: Jan 4, 2025
 *      Author: Ln-Komandur
 */
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>

#ifndef MAIN_PCF8574_IO_EXPANDER_H_
#define MAIN_PCF8574_IO_EXPANDER_H_

class PCF8574_IO_Expander {
	private:
	
		i2c_master_dev_handle_t device_handle;
		const char *TAG = "PCF8574_IO_Expander";
		esp_err_t err;


	public:
		PCF8574_IO_Expander();// Forced to have an empty / default constructor
		PCF8574_IO_Expander(i2c_master_bus_handle_t bus_handle, uint8_t jumper_address);
		void write_byte_to_pins(uint8_t data); // write one byte to pins
		uint8_t read_byte_from_pins(); // read one byte from pins
	
};

#endif /* MAIN_PCF8574_IO_EXPANDER_H_ */
