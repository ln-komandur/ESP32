// Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/

// Yet to be implemented

#include <stdio.h>
#include "i2c-matrix-keypad.h"
#include "esp_log.h"
#include "driver/i2c.h"

#define PCF8574_SLAVE_KPD_ADDRESS 0x20 // No jumper set for keypad on PCF8574 expander

#define TIMEOUT_MS		1000
// #define DELAY_MS		1000

esp_err_t err;

static const char *TAG = "Matrix Keypad";

// Set the Key at Use (4x4)
char keys [4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

uint8_t get_keypad_pins() {
	//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
	//  Reading from read_from_PCF8574_pins
	uint8_t rx_data[1];

	i2c_master_read_from_device(I2C_NUM_0, PCF8574_SLAVE_KPD_ADDRESS, rx_data, 5, TIMEOUT_MS/portTICK_RATE_MS);

	ESP_LOGI(TAG, "get_keypad_pins");
	ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);

	return rx_data[0]; // there is only one element in this array. Return it.
}

void set_keypad_pins(uint8_t data)
{
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	err = i2c_master_write_to_device(I2C_NUM_0, PCF8574_SLAVE_KPD_ADDRESS, data_t, len, 1000);
	if (err!=0)
		ESP_LOGI(TAG, "Error in sending data to PCF8574 keypad");
	else
		ESP_LOGI(TAG, "set_keypad_pins successful");

}

char find_key(){

	uint8_t byteOfColumns; //  used for storing what is read from PCF8574 keypad
	uint8_t bitSetOnColumn = 0;
	int col = 0;

	byteOfColumns = get_keypad_pins();
	// find the column
	bitSetOnColumn = (255 - byteOfColumns)&0xf0;
	// set that column to 1 and others to 0
	switch (bitSetOnColumn) {
		case 128: 	// bitSetOnColumn = 0x80: 10000000. byteOfColumns = 01110000 = 112 = 0x70
			col = 1; break;
		case 64: 	// bitSetOnColumn = 0x40: 01000000. byteOfColumns = 10110000 = 176 = 0xb0
			col = 2; break;
		case 32: 	// bitSetOnColumn = 0x20: 00100000. byteOfColumns = 11010000 = 208 = 0xd0
			col = 3; break;
		case 16: 	// bitSetOnColumn = 0x10: 00010000. byteOfColumns = 11100000 = 224 = 0xe0
			col = 4; break;
	}

	set_keypad_pins(0x0f); // set all row pins high now

	// find the row
	uint8_t byteOfRows; //  used for storing what is read from PCF8574 keypad

	byteOfRows = get_keypad_pins();

	uint8_t bitSetOnRow;
	bitSetOnRow = (15 - byteOfRows)&0x0f;
	char keyPressed = 0;
	switch (bitSetOnRow) {
		case 8: 	// bitSetOnRow = 0x08: 00001000. byteOfRows = 00000111 = 7 = 0x07
			keyPressed = keys[col-1][0];break;
		case 4: 	// bitSetOnRow = 0x04: 00000100. byteOfRows = 00001011 = 11 = 0x0b
			keyPressed = keys[col-1][1];break;
		case 2: 	// bitSetOnRow = 0x02: 00000010. byteOfRows = 00001101 = 13 = 0x0d
			keyPressed = keys[col-1][2];break;
		case 1: 	// bitSetOnRow = 0x01: 00000001. byteOfRows = 00001110 = 14 = 0x0e
			keyPressed = keys[col-1][3];break;
	}
	set_keypad_pins(0xf0); // write 11110000 and re-initialize the keypad as both the column and the row are found by now
	return keyPressed;
}
