/*
 * i2c_mast_handler.c
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 *
 * Manages (methods to initialize, scan) the i2c buses for ESP32 and devices connected to it
 * Reference 
 * - https://embeddedexplorer.com/esp32-i2c-tutorial/ - The code here is originally from https://gist.github.com/herzig/8d4c13d8b81a77ac86481c6c1306bb12
 * - https://github.com/espressif/esp-idf/blob/master/examples/peripherals/i2c/i2c_tools/main/cmd_i2ctools.c
 */


#include <i2c_mast_handler.h>
#include <stdint.h>

// I2C Clock Speed - standard mode: 100 kbit/s, full speed: 400 kbit/s,... per https://www.i2c-bus.org/speed/
//#define MSTR_CLK_SPD		100000 //  400KHz solve'D' https://arduino.stackexchange.com/questions/19150/i2c-lcd-displaying-weird-characters

static const char *TAG = "i2c_mast_handler";

/*
 * Initializes an i2c bus at the given i2c port number and returns the bus handle
 */
i2c_master_bus_handle_t get_i2c_bus_handle(int i2c_num)
{
	i2c_master_bus_handle_t bus_handle;
	// Provide the master bus config - begin
	i2c_master_bus_config_t master_cfg = {
	    .clk_source = I2C_CLK_SRC_DEFAULT,
	    .i2c_port = i2c_num,
   		.scl_io_num = GPIO_NUM_22, // The SCL pins of these instances are not hard-wired to any specific GPIO pin. They are mapped to physical GPIO pins through software configuration.
   		.sda_io_num = GPIO_NUM_21, // The SDA pins of these instances are not hard-wired to any specific GPIO pin. They are mapped to physical GPIO pins through software configuration.
	    .glitch_ignore_cnt = 7,
	    .flags.enable_internal_pullup = true,
	};
	// Provide the master bus config - end
	
	// Create a new master bus with the bus handle and config
 	i2c_new_master_bus(&master_cfg, &bus_handle); // return this if it is in a method as it will show success or error

	ESP_LOGI(TAG, "i2c_master_init: Got i2c Bus Handle: About to return that BUS handle");
	return bus_handle;
}
/*
 * Returns a pointer to an array of i2c_device_addresses for the given bus handle
 */
uint8_t* get_connected_i2c_device_addresses(i2c_master_bus_handle_t bus_handle)
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
