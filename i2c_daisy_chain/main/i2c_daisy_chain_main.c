/* i2c - Daisy chain example
 * LCD	1602 display, 4x4 Matrix Keypad and 8 LED lights daisy-chained over I2C using PCF8574 GPIO expander
 * Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/
 */


#include <i2c_LCD.h>
#include <PCF8574.h>
#include <a_byte_of_LEDs.h>
#include <i2c-matrix-keypad.h>
#include <config_i2c_port.h>
#include <esp_log.h>

#define I2C_NUM I2C_NUM_0 
// This is the I2C hardware modules (or I2C ports) in the ESP32 are we using to facilitate I2C communication. We can use I2C_NUM_0 or I2C_NUM_1


static const char *TAG = "i2c-daisy-chain-example";

long blinkDuration = 500000; // 500000 is in microseconds for 0.5 seconds

uint8_t byteToBlinkLEDs = 0x55; // Binary of 0x55 is 01010101. This is the byte that LEDs will blink for by shifting left.

QueueHandle_t keyQueue;

void app_main(void)
{

	ESP_ERROR_CHECK(i2c_master_init(I2C_NUM));
	ESP_LOGI(TAG, "I2C initialized successfully via i2c_port.c");
	scan_for_i2c_devices(I2C_NUM);
	lcd_init(I2C_NUM);
	lcd_clear();

	write_string_on_LCD(0,0, "Matrix keypad");

	init_keypad(I2C_NUM);

	attach_queue_to_LCD(keyQueue);

	init_LED_blink(I2C_NUM,byteToBlinkLEDs, blinkDuration);
}
