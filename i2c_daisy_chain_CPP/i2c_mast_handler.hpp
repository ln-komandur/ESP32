/*
 * i2c_mast_handler.hpp
 *
 *  Created on: Jan 4, 2025
 *      Author: Ln-Komandur
 */
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <esp_timer.h>


#ifndef MAIN_i2c_mast_handler_H_
#define MAIN_i2c_mast_handler_H_
#define MAX_NO_OF_I2C_DEVICES 128 // This is the maximum number of I2C devices that can be connected

class i2c_mast_handler {
	private:
		i2c_master_bus_handle_t bus_handle; // Handle for the master bus
		const char *TAG = "i2c_mast_handler";
		esp_err_t err;
	public:

	 	i2c_mast_handler(int i2c_num, gpio_num_t scl_pin_no, gpio_num_t sda_pin_no, bool isInternalPullupEnabled);
	 	i2c_master_bus_handle_t get_i2c_bus_handle();
	 	uint8_t * get_connected_i2c_device_addresses();
};

#endif /* MAIN_i2c_mast_handler_ */
