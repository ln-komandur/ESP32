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



void app_main(void)
{

	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully via i2c_port.c");
	scan_for_i2c_devices();
	lcd_init();
	lcd_clear();

	write_string_on_LCD(0,0, "Matrix keypad");

	setup_keypad();

	init_LCD(keyQueue);

	init_LED_Byte(0x55); //  binary of 0x55 is 01010101
}
