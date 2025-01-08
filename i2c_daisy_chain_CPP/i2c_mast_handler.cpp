/*
 * i2c_mast_handler.cpp
 *
 *  Created on: Jan 4, 2025
 *      Author: Ln-Komandur
 */
#include <i2c_mast_handler.hpp>

i2c_mast_handler::i2c_mast_handler(int i2c_num, gpio_num_t scl_pin_no, gpio_num_t sda_pin_no, bool isInternalPullupEnabled)
{
	// Provide the master bus config - begin
	i2c_master_bus_config_t master_cfg = {};
	    master_cfg.clk_source = I2C_CLK_SRC_DEFAULT;
	    master_cfg.i2c_port = i2c_num;
   		master_cfg.scl_io_num = scl_pin_no; // The SCL pins of these instances are not hard-wired to any specific GPIO pin. They are mapped to physical GPIO pins through software configuration.
   		master_cfg.sda_io_num = sda_pin_no, // The SDA pins of these instances are not hard-wired to any specific GPIO pin. They are mapped to physical GPIO pins through software configuration.
	    master_cfg.glitch_ignore_cnt = 7;
	    master_cfg.flags.enable_internal_pullup = isInternalPullupEnabled;
	// Provide the master bus config - end

	ESP_LOGI(TAG, "i2c_master_init: About to return");

	// Create a new master bus with the bus handle and config
 	err = i2c_new_master_bus(&master_cfg, &bus_handle); // return this if it is in a method as it will show success or error. But a constructor does not return anything
}


i2c_master_bus_handle_t i2c_mast_handler::get_i2c_bus_handle()
{
	return bus_handle;
}

/*
 * Returns a pointer to an array of i2c_device_addresses for the given bus handle
 */
uint8_t* i2c_mast_handler::get_connected_i2c_device_addresses()
{
	static uint8_t connectedAddresses[MAX_NO_OF_I2C_DEVICES];
	int deviceCount = 0;
	ESP_LOGI(TAG, "get_connected_i2c_device_addresses: scanning all addresses");

	for (uint8_t address = 0; address < MAX_NO_OF_I2C_DEVICES; address++)
    {
        esp_err_t ret = i2c_master_probe(bus_handle, address, 100 / portTICK_PERIOD_MS);
        if (ret == ESP_OK)
		{
			connectedAddresses[deviceCount++] = address; // If a device is found, put its address in the byte array	
		}
    }
   	ESP_LOGI(TAG, "get_connected_i2c_device_addresses: returning an array of device addresses found");

    return connectedAddresses; // return the array of addresses 
}

