// Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/

// Yet to be implemented

#include <stdio.h>
#include "i2c-matrix-keypad.h"

#include "esp_log.h"

#include "driver/i2c.h"

#define PCF8574_SLAVE_KPD_ADDRESS 0x20 // No jumper set for keypad on PCF8574 expander

#define TIMEOUT_MS		1000
#define DELAY_MS		1000


esp_err_t err;

static const char *TAG = "Matrix Keypad";

uint8_t read_keypad_pins() {
	//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
	//  Reading from read_from_PCF8574_pins
	uint8_t rx_data[1];

	i2c_master_read_from_device(I2C_NUM_0, PCF8574_SLAVE_KPD_ADDRESS, rx_data, 5, TIMEOUT_MS/portTICK_RATE_MS);
	vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	ESP_LOGI(TAG, "read_keypad_pins");
	ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);

	return rx_data[0]; // there is only one element in this array. Return it.
}

void write_to_keypad(uint8_t data)
{
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	err = i2c_master_write_to_device(I2C_NUM_0, PCF8574_SLAVE_KPD_ADDRESS, data_t, len, 1000);

	vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	if (err!=0)
		ESP_LOGI(TAG, "Error in sending data to PCF8574 keypad");
	else
		ESP_LOGI(TAG, "write_to_keypad successful");

}
