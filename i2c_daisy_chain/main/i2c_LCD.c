#include "i2c_port.h"
#include "i2c_LCD.h"

#include "esp_log.h"
#include "driver/i2c.h"
#include "unistd.h"

//#define LCD_SLAVE_ADDR 0x4E>>1 // change this according to your device setup
#define LCD_SLAVE_ADDR 0x27 // This is the same as 0x4E>>1


esp_err_t err;

#define I2C_NUM I2C_NUM_0

static const char *TAG = "LCD";

void lcd_send_cmd (char cmd)
{
  	char upper_nibble, lower_nibble;
	uint8_t data_t[4];
	upper_nibble = (cmd&0xf0);
	lower_nibble = ((cmd<<4)&0xf0);
	// rs=0 is command register. Refer https://embeddedexpert.io/?p=600
	// refer https://embeddedexpert.io/?p=655 for RS and EN pin
	data_t[0] = upper_nibble|0x0C;  //en=1, rs=0
	data_t[1] = upper_nibble|0x08;  //en=0, rs=0
	data_t[2] = lower_nibble|0x0C;  //en=1, rs=0
	data_t[3] = lower_nibble|0x08;  //en=0, rs=0
	err = i2c_master_write_to_device(I2C_NUM, LCD_SLAVE_ADDR, data_t, 4, 1000);
	if (err!=0) ESP_LOGI(TAG, "Error writing command to LCD");
}

void lcd_send_data (char data)
{
	char upper_nibble, lower_nibble;
	uint8_t data_t[4];
	upper_nibble = (data&0xf0);
	lower_nibble = ((data<<4)&0xf0);
	data_t[0] = upper_nibble|0x0D;  //en=1, rs=0
	data_t[1] = upper_nibble|0x09;  //en=0, rs=0
	data_t[2] = lower_nibble|0x0D;  //en=1, rs=0
	data_t[3] = lower_nibble|0x09;  //en=0, rs=0
	err = i2c_master_write_to_device(I2C_NUM, LCD_SLAVE_ADDR, data_t, 4, 1000);
	if (err!=0) ESP_LOGI(TAG, "Error writing data to LCD");
}

void lcd_clear (void)
{
	lcd_send_cmd (0x01);
	usleep(5000);
}

void lcd_put_cur(int row, int col)
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

    	lcd_send_cmd (col);
}


/*
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller.
 * Its command set is a useful reference.
 * The initialisation pattern is shown in page 46 of the datasheet at
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 * Also refer https://mil.ufl.edu/3744/docs/lcdmanual/commands.html#Sda
 *
 */

void lcd_init (void)
{

	// 4 bit initialisation
	usleep(50000);  // wait for >40ms
	lcd_send_cmd (0x30);
	usleep(5000);  // wait for >4.1ms
	lcd_send_cmd (0x30);
	usleep(200);  // wait for >100us
	lcd_send_cmd (0x30);
	usleep(10000);
	lcd_send_cmd (0x20);  // 4bit mode
	usleep(10000);

  	// display initialisation
	lcd_send_cmd (0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	usleep(1000);
	lcd_send_cmd (0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	usleep(1000);
	lcd_send_cmd (0x01);  // clear display
	usleep(1000);
	usleep(1000);
	lcd_send_cmd (0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	usleep(1000);
	lcd_send_cmd (0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	usleep(1000);
}

void lcd_send_string (char *str)
{
	while (*str) lcd_send_data (*str++);
}
