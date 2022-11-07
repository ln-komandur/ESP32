/*
 * Wrapper i2c functions for reading from and writing to PCF8574
 * Examples can be to light up LEDs on i2c.
 * LED light on each pin of the PCF8574 can blink based on bits written to them.
 * Pins can also be read likewise.
*/

#include <PCF8574.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define PCF8574_SLAVE_ADDR 0x21 // A0 jumper to ground

#define TIMEOUT_MS		1000
#define DELAY_MS		1000


static const char *TAG = "PCF8574";

uint8_t read_byte_from_pins()
{

//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
//  Reading from read_from_PCF8574_pins
	uint8_t rx_data[1];

	i2c_master_read_from_device(I2C_NUM_0, PCF8574_SLAVE_ADDR, rx_data, 1, TIMEOUT_MS/portTICK_RATE_MS);
	ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);
	vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	ESP_LOGI(TAG, "read_byte_from_pins");

	return rx_data[0]; // there is only one element in this array. Return it.
}

void write_byte_to_pins(uint8_t data)
{
	esp_err_t err;
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	err = i2c_master_write_to_device(I2C_NUM_0, PCF8574_SLAVE_ADDR, data_t, len, 1000);

	vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	if (err!=0)
		ESP_LOGI(TAG, "Error in sending data to PCF8574");
	else
		ESP_LOGI(TAG, "Write_bytes_to_pins successful");

}
