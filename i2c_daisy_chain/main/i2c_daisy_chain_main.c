/* i2c - Daisy chain example

   Simple I2C example showing 8 LED lights on PCF8574 and LCD1602 on PCF8574 daisy-chained over I2C

*/
#include <i2c_port.h>
#include <i2c_LCD.h>
#include <PCF8574.h>
#include <i2c-matrix-keypad.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define DELAY_MS		1000

static const char *TAG = "i2c-daisy-chain-example";


void write_string_on_LCD(int lineNo, int colNo, char *str)
{
	lcd_put_cur(lineNo, colNo);
	lcd_send_string("                "); // Erases the existing content fully

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
	uint8_t aByte = 0x55; //  binary of 0x55 is 01010101
	uint8_t aFoundByte[1]; //  used for storing what is read from PCF8574
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
	while (true)
	{
		write_byte_to_pins(aByte);
		aByte = aByte << 1;
		if (aByte == 0x00) {
			aByte = 0x55;
		}
		aFoundByte[0] = read_byte_from_pins();
		ESP_LOGI(TAG, "Read byte from PCF8574 pins");
		ESP_LOG_BUFFER_HEX(TAG, aFoundByte , 1);

		write_hex_on_LCD(1, 0, aFoundByte[0]); // display it on line 2 of the LCD though as decimal
	}
}

void show_byte_with_LEDs(uint8_t aByte)
{
	write_byte_to_pins(aByte);
	ESP_LOGI(TAG, "show_byte_with_LEDs");
}

void showKeyPressed(void) // this method may be called based on interrupt. Yet to explore
{
	char pressedKey; //  used for storing what is read from PCF8574 keypad
	pressedKey = find_key();

	if (pressedKey != 0) // this will go away when doing interrupts, as we come to this function only when the key is pressed
	{
		ESP_LOGI(TAG, "Key Pressed on PCF8574 keypad ");
		char buffer[16];
		sprintf(buffer, "Pressed %c", pressedKey); // display hexadecimal
		write_string_on_LCD(1, 0, buffer); // display it on line 2 of the LCD though as string
	} else // this will go away when doing interrupts, as we come to this function only when the key is pressed
	{
		ESP_LOGI(TAG, "Key NOT Pressed on PCF8574 keypad ");
		write_string_on_LCD(1, 0, "None pressed "); // display it on line 2 of the LCD though as string
	}


}

void app_main(void)
{
	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully via i2c_port.c");
	scan_for_i2c_devices();
	lcd_init();
	lcd_clear();

	write_string_on_LCD(0,0, "Matrix keypad");

	set_keypad_pins(0xf0); // write 11110000

	while (true)
	{
		showKeyPressed();
		vTaskDelay(DELAY_MS/portTICK_RATE_MS); // Show the message on LCD for 1 second
	}
	//blink_LEDs_by_bit_shift();
}
