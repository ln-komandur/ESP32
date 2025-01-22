/*
 * i2c_LCD.c
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller.
 * Refer
 * - https://mil.ufl.edu/3744/docs/lcdmanual/commands.html#Sda
 * - https://exploreembedded.com/wiki/Interfacing_LCD_in_4-bit_mode_with_8051
 * - https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 *
 */

#include <i2c_LCD.h>
#include <sys/unistd.h>

#define REFRESH_MS	1000 // In how many milliseconds should the clock time be refreshed

esp_err_t err;

static const char *TAG = "i2c_LCD";

i2c_master_dev_handle_t get_i2c_device_handle(i2c_master_bus_handle_t bus_handle, uint8_t dev_address)
{
	i2c_master_dev_handle_t device_handle;

	// Configure the device at a certain address
	i2c_device_config_t pcf8574_cfg = {
	    .dev_addr_length = I2C_ADDR_BIT_LEN_7,
	    .device_address = dev_address,
	    .scl_speed_hz = 400000,
	    .scl_wait_us = 0, // Clock stretch set to 0 to use default register value per https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/i2c.html 
	    //.scl_wait_us = 5 * 1000, // BQ27441 can stretch up to 4ms, use 5ms to be safe. From https://github.com/espressif/esp-idf/issues/14401
	};
	// Add the device to the master bus
	i2c_master_bus_add_device(bus_handle, &pcf8574_cfg, &device_handle);
	ESP_LOGI(TAG, "get_i2c_device_handle: Got a Device Handle: About to return that DEVICE handle");
	return device_handle; // Return the device handle
}

void lcd_send_cmd (i2c_master_dev_handle_t LCD_dev_handle, char cmd)
{
  	char u_cmd, l_cmd;
	uint8_t cmd_t[4];
	u_cmd = (cmd&0xf0); // this will give a byte that begins with the upper 4 bits of cmd, and have the lower 4 bits as 0000
	l_cmd = ((cmd<<4)&0xf0); // this will give a byte that begins with the lower 4 bits of cmd, and have the lower 4 bits as 0000
	// RS=0 is to select command register. Refer https://embeddedexpert.io/?p=600
	// refer https://embeddedexpert.io/?p=655 for RS and EN pin
	/* Pin connection order in the notation LCD connection (to PCF8574 pin) is
	 * D7(P7), D6(P6), D5(P5), D4(P4), VEE(P3), EN(P2), RW (P1), RS (P0)
	 * Refer https://www.instructables.com/HD44780-LCD-to-I2C-adapter-board-for-the-Bus-Pirat/
	 * The VEE pin regulates the display's contrast, via a changeable POT that can supply 0 to 5V. https://www.elprocus.com/lcd-16x2-pin-configuration-and-its-working/
	 */

	cmd_t[0] = u_cmd|0x0C;  // Lower 4 bits are 1100. VEE(P3)=1, EN(P2)=1, RW(P1)=0, RS(P0)=0.
	cmd_t[1] = u_cmd|0x08;  // Lower 4 bits are 1000. VEE(P3)=1, EN(P2)=0, RW(P1)=0, RS(P0)=0
	cmd_t[2] = l_cmd|0x0C;  // Lower 4 bits are 1100. VEE(P3)=1, EN(P2)=1, RW(P1)=0, RS(P0)=0
	cmd_t[3] = l_cmd|0x08;  // Lower 4 bits are 1000. VEE(P3)=1, EN(P2)=0, RW(P1)=0, RS(P0)=0

	// Transmit / Write to the device
	int len = 4;
	err = i2c_master_transmit(LCD_dev_handle, cmd_t, len, -1); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	
	if (err!=0) ESP_LOGI(TAG, "Error writing command to LCD");
}

void lcd_send_data (i2c_master_dev_handle_t LCD_dev_handle, char data)
{
	char u_data, l_data;
	uint8_t data_t[4];
	u_data = (data&0xf0); // this will give a byte that begins with the upper 4 bits of data, and have the lower 4 bits as 0000
	l_data = ((data<<4)&0xf0); // this will give a byte that begins with the lower 4 bits of data, and have the lower 4 bits as 0000
	// RS=1 is to select data register. Refer https://exploreembedded.com/wiki/Interfacing_LCD_in_4-bit_mode_with_8051
	/* Pin connection order in the notation LCD connection (to PCF8574 pin) is
	 * D7(P7), D6(P6), D5(P5), D4(P4), VEE(P3), EN(P2), RW (P1), RS (P0)
	 * The VEE pin regulates the display's contrast, via a changeable POT that can supply 0 to 5V. https://www.elprocus.com/lcd-16x2-pin-configuration-and-its-working/
	 */

	data_t[0] = u_data|0x0D;  // Lower 4 bits are 1101. VEE(P3)=1, EN(P2)=1, RW(P1)=0, RS(P0)=1.
	data_t[1] = u_data|0x09;  // Lower 4 bits are 1001. VEE(P3)=1, EN(P2)=0, RW(P1)=0, RS(P0)=1.
	data_t[2] = l_data|0x0D;  // Lower 4 bits are 1101. VEE(P3)=1, EN(P2)=1, RW(P1)=0, RS(P0)=1.
	data_t[3] = l_data|0x09;  // Lower 4 bits are 1001. VEE(P3)=1, EN(P2)=0, RW(P1)=0, RS(P0)=1.

	// Transmit / Write to the device
	int len = 4;
	err = i2c_master_transmit(LCD_dev_handle, data_t, len, -1); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	if (err!=0) ESP_LOGI(TAG, "Error writing data to LCD");
}

void lcd_clear (i2c_master_dev_handle_t LCD_dev_handle)
{
	lcd_send_cmd (LCD_dev_handle, 0x01);
	usleep(5000);
}

void lcd_put_cur(i2c_master_dev_handle_t LCD_dev_handle,int row, int col)
{
	switch (row)
	{
	case 0:
		col |= 0x80;
		break;
	case 1:
		col |= 0xC0;
		break;
	}
	lcd_send_cmd(LCD_dev_handle, col);
}


/*
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller,
 * whose command set is a useful reference.
 * The initialisation pattern is shown in page 46 of the datasheet at
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 */

void lcd_init (i2c_master_dev_handle_t LCD_dev_handle)
{
	// 4 bit initialisation
	usleep(50000);  // wait for >40ms
	lcd_send_cmd (LCD_dev_handle, 0x30);
	usleep(5000);  // wait for >4.1ms
	lcd_send_cmd (LCD_dev_handle, 0x30);
	usleep(200);  // wait for >100us
	lcd_send_cmd (LCD_dev_handle, 0x30);
	usleep(10000);
	lcd_send_cmd (LCD_dev_handle, 0x20);  // 4bit mode
	usleep(10000);

  	// display initialisation
	lcd_send_cmd (LCD_dev_handle, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	usleep(1000);
	lcd_send_cmd (LCD_dev_handle, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	usleep(1000);
	lcd_send_cmd (LCD_dev_handle, 0x01);  // clear display
	usleep(1000);
	usleep(1000);
	lcd_send_cmd (LCD_dev_handle, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	usleep(1000);
	lcd_send_cmd (LCD_dev_handle, 0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	usleep(1000);
	ESP_LOGI(TAG, "Initialized LCD");

}

void lcd_send_string (i2c_master_dev_handle_t LCD_dev_handle, char *str)
{
	while (*str) lcd_send_data (LCD_dev_handle, *str++);
}

void write_string_on_LCD(i2c_master_dev_handle_t LCD_dev_handle, int lineNo, int colNo, char *str)
{
	lcd_put_cur(LCD_dev_handle, lineNo, colNo);
	lcd_send_string(LCD_dev_handle, "				"); // Erases the existing content fully

	lcd_put_cur(LCD_dev_handle, lineNo, colNo);
	lcd_send_string(LCD_dev_handle, str);
}

void write_hex_on_LCD(i2c_master_dev_handle_t LCD_dev_handle, int lineNo, int colNo, uint8_t hex)
{
	char buffer[16];
	sprintf(buffer, "0x%02X", hex); // display hexadecimal
	
	lcd_put_cur(LCD_dev_handle, lineNo, colNo);
	lcd_send_string(LCD_dev_handle, buffer);
}
