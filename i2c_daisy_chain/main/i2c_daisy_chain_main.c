/* i2c - Daisy chain example
 * LCD	1602 display, 4x4 Matrix Keypad and 8 LED lights daisy-chained over I2C using PCF8574 GPIO expander
 */

#include <i2c_port.h>
#include <i2c_LCD.h>
#include <PCF8574.h>
#include <i2c-matrix-keypad.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_timer.h"

#define DELAY_MS		1000

static const char *TAG = "i2c-daisy-chain-example";

uint8_t aByte = 0x55; //  binary of 0x55 is 01010101

int64_t lastBlink;
long blinkDuration = 500000; // 500000 is in microseconds for 0.5 seconds

void write_string_on_LCD(int lineNo, int colNo, char *str)
{
	lcd_put_cur(lineNo, colNo);
	lcd_send_string("				"); // Erases the existing content fully

	lcd_put_cur(lineNo, colNo);
	lcd_send_string(str);
}

void write_hex_on_LCD(int lineNo, int colNo, uint8_t hex)
{
	char buffer[16];
	sprintf(buffer, "0x%02X", hex); // display hexadecimal

	lcd_put_cur(lineNo, colNo);
	lcd_send_string(buffer);
}

void blink_LEDs_by_bit_shift()
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

void show_byte_with_LEDs(uint8_t aByte)
{
	write_byte_to_pins(aByte);
	ESP_LOGI(TAG, "show_byte_with_LEDs");
}


void app_main(void)
{

	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully via i2c_port.c");
	scan_for_i2c_devices();
	lcd_init();
	lcd_clear();

	write_string_on_LCD(0,0, "Matrix keypad");

	setup_keypad();

	lastBlink = esp_timer_get_time();

	while (true)
	{
		char keyPressed = 0;

		xQueueReceive(keyQueue, &keyPressed, ( portTickType ) 0); // put the key found in the queue
		if (keyPressed != 0)
		{
			char buffer[16];
			sprintf(buffer, "Last pressed %c", keyPressed); // display hexadecimal
			write_string_on_LCD(1, 0, buffer); // display it on line 2 of the LCD though as string
			// vTaskDelay(DELAY_MS/portTICK_RATE_MS); // Show the key name on the LCD for a short time

		}

		blink_LEDs_by_bit_shift();

	}

}
