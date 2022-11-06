/* i2c - Daisy chain example

   Simple I2C example showing 8 LED lights on PCF8574 and LCD1602 on PCF8574 daisy chained over I2C

*/
#include <i2c_port.h>
#include <i2c_LCD.h>
#include <i2c_LED_lights.h>

#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c-daisy-chain-example";


void write_string_on_LCD(int lineNo, int colNo, char *str)
{
	lcd_put_cur(lineNo, colNo);
	lcd_send_string(str);
}

void write_non_string_on_LCD(int lineNo, int colNo, float num)
{
	char buffer[16];
	sprintf(buffer, "%.11f", num); // display upto 11 digits after decimal point

	lcd_put_cur(lineNo, colNo);
	lcd_send_string(buffer);

}


void app_main(void)
{
	ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully via i2c_port.c");
    scan_for_i2c_devices();
	lcd_init();
	lcd_clear();

    write_string_on_LCD(0,0, "Hello ESP32 !!");

    write_non_string_on_LCD(1, 0, 3.1415926536);
    uint8_t aByte = 0x55; //  binary of 0x55 is 01010101
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
    	write_to_PCF8574_pins(aByte);
    	aByte = aByte << 1;
    	if (aByte == 0x00) {
    		aByte = 0x55;
    	}
    	read_from_PCF8574_pins(); // when this function is modified to return the Byte, display it on Row 2 of LCD
    }

}
