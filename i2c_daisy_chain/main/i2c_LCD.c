/*
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller.
 * Refer
 * - https://mil.ufl.edu/3744/docs/lcdmanual/commands.html#Sda
 * - https://exploreembedded.com/wiki/Interfacing_LCD_in_4-bit_mode_with_8051
 * - https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 *
 */


#include <i2c_LCD.h>

//#define LCD_SLAVE_ADDR 0x4E>>1 // change this according to your device setup
#define LCD_SLAVE_ADDR 0x27 // This is the same as 0x4E>>1


esp_err_t err;

#define I2C_NUM I2C_NUM_0

static const char *TAG = "LCD";

QueueHandle_t characterQueue;


void lcd_send_cmd (char cmd)
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


	err = i2c_master_write_to_device(I2C_NUM, LCD_SLAVE_ADDR, cmd_t, 4, 1000);
	if (err!=0) ESP_LOGI(TAG, "Error writing command to LCD");
}

void lcd_send_data (char data)
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
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller,
 * whose command set is a useful reference.
 * The initialisation pattern is shown in page 46 of the datasheet at
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
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


void LCD_Receiver_Task(void *params)
{
	while (true)
	{
		char keyPressed = 0;

		xQueueReceive(characterQueue, &keyPressed, ( TickType_t ) 0); // put the key found in the queue
		if (keyPressed != 0)
		{
			char buffer[16];
			sprintf(buffer, "Last pressed %c", keyPressed); // display hexadecimal
			write_string_on_LCD(1, 0, buffer); // display it on line 2 of the LCD though as string
		}

	}

}


void attach_queue_to_LCD(QueueHandle_t keyQueue)
{
	xTaskCreate(LCD_Receiver_Task, "LCD_Receiver_Task", 2048, NULL, 1, NULL);
	characterQueue = keyQueue; // this queue holds the keys pressed. Its size as 32, is the total number of characters on a 1602 LCD display
}

