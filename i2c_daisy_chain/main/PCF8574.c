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

esp_err_t err;

uint8_t aByte;

int64_t lastBlink;
long blinkDuration; // in microseconds

uint8_t read_byte_from_pins()
{

//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
//  Reading from read_from_PCF8574_pins
	uint8_t rx_data[1];

	i2c_master_read_from_device(I2C_NUM_0, PCF8574_SLAVE_ADDR, rx_data, 1, TIMEOUT_MS/portTICK_RATE_MS);
	ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);

	ESP_LOGI(TAG, "read_byte_from_pins");

	return rx_data[0]; // there is only one element in this array. Return it.
}

void write_byte_to_pins(uint8_t data)
{
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	err = i2c_master_write_to_device(I2C_NUM_0, PCF8574_SLAVE_ADDR, data_t, len, 1000);

	if (err!=0) ESP_LOGI(TAG, "Error in sending data to PCF8574");
	/* else
		ESP_LOGI(TAG, "Write_bytes_to_pins successful");
	*/
}

void show_byte_with_LEDs(uint8_t aByte)
{
	write_byte_to_pins(aByte);
	ESP_LOGI(TAG, "show_byte_with_LEDs");
}
void blink_LEDs_Task(void *params)
{
	lastBlink = esp_timer_get_time();

	while (true)
	{
	    int64_t callTime = esp_timer_get_time();

		/*
		 * 0x55 = 01010101, which,
		 * when shifted left once, it gives 0xaa = 10101010, which
		 * when shifted left once, it gives 0x54 = 01010100, which
		 * when shifted left once, it gives 0xa8 = 10101000, which
		 * when shifted left once, it gives 0x50 = 01010000, which
		 * when shifted left once, it gives 0xa0 = 10100000, which
		 * when shifted left once, it gives 0x40 = 01000000, which
		 * when shifted left once, it gives 0x80 = 10000000, which
		 * when shifted left once, it gives 0x00 = 00000000, when the byte is reset to 0x55
		 */
		if (callTime - lastBlink > blinkDuration)
		{
			write_byte_to_pins(aByte);
			aByte = aByte << 1;
			if (aByte == 0x00) {
				aByte = 0x55;
			}
			lastBlink = callTime;
		}
	}
}


void init_LED(uint8_t startByte, long duration)
{
	xTaskCreate(blink_LEDs_Task, "blink_LEDs_Task", 2048, NULL, 1, NULL);
	blinkDuration = duration;
	aByte = startByte;

}
