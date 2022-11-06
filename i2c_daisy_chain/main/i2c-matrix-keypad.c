// Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/

// Yet to be implemented

/*
#include <i2c_LCD.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define SLAVE_ADDRESS_KPD 0x21 // A0 jumper high for keypad

#define I2C_SLAVE_ADDR	0x11
#define TIMEOUT_MS		1000
#define DELAY_MS		1000


esp_err_t err;

#define I2C_NUM I2C_NUM_0

static const char *TAG = "Matrix Keypad";

void read_keypad_app_main() {
	uint8_t rx_data[5];

	while (1) {
		i2c_master_read_from_device(I2C_NUM, I2C_SLAVE_ADDR, rx_data, 5, TIMEOUT_MS/portTICK_RATE_MS);
		ESP_LOG_BUFFER_HEX(TAG, rx_data, 5);
		vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	}
}
*/
