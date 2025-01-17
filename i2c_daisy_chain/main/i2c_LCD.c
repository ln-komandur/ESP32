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

#define COUNTER_MS	1000 // In how many milliseconds should the counter be refreshed

esp_err_t err;

static const char *TAG = "i2c_LCD";



void lcd_send_cmd (struct LCD_Setup LCD_cfg, char cmd)
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
	err = i2c_master_transmit(LCD_cfg.device_handle, cmd_t, len, -1); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	
	if (err!=0) ESP_LOGI(TAG, "Error writing command to LCD");
}

void lcd_send_data (struct LCD_Setup LCD_cfg, char data)
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
	err = i2c_master_transmit(LCD_cfg.device_handle, data_t, len, -1); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	if (err!=0) ESP_LOGI(TAG, "Error writing data to LCD");
}

void lcd_clear (struct LCD_Setup LCD_cfg)
{
	lcd_send_cmd (LCD_cfg, 0x01);
	usleep(5000);
}

void lcd_put_cur(struct LCD_Setup LCD_cfg,int row, int col)
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
	lcd_send_cmd(LCD_cfg, col);
}


/*
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller,
 * whose command set is a useful reference.
 * The initialisation pattern is shown in page 46 of the datasheet at
 * https://www.sparkfun.com/datasheets/LCD/HD44780.pdf
 *
 */

void lcd_init (struct LCD_Setup LCD_cfg)
{
	// 4 bit initialisation
	usleep(50000);  // wait for >40ms
	lcd_send_cmd (LCD_cfg, 0x30);
	usleep(5000);  // wait for >4.1ms
	lcd_send_cmd (LCD_cfg, 0x30);
	usleep(200);  // wait for >100us
	lcd_send_cmd (LCD_cfg, 0x30);
	usleep(10000);
	lcd_send_cmd (LCD_cfg, 0x20);  // 4bit mode
	usleep(10000);

  	// display initialisation
	lcd_send_cmd (LCD_cfg, 0x28); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
	usleep(1000);
	lcd_send_cmd (LCD_cfg, 0x08); //Display on/off control --> D=0,C=0, B=0  ---> display off
	usleep(1000);
	lcd_send_cmd (LCD_cfg, 0x01);  // clear display
	usleep(1000);
	usleep(1000);
	lcd_send_cmd (LCD_cfg, 0x06); //Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
	usleep(1000);
	lcd_send_cmd (LCD_cfg, 0x0C); //Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
	usleep(1000);

	xTaskCreate(&LCD_Queue_Receiver_Task, "LCD_Queue_Receiver_Task", 2048, &LCD_cfg, 1, NULL);
	ESP_LOGI(TAG, "Created LCD_Queue_Receiver_Task");

	xTaskCreate(&LCD_Counter_Task, "LCD_Counter_Task", 2048, &LCD_cfg, 1, NULL);
	ESP_LOGI(TAG, "Created LCD_Counter_Task");

}

void lcd_send_string (struct LCD_Setup LCD_cfg, char *str)
{
	while (*str) lcd_send_data (LCD_cfg, *str++);
}

void write_string_on_LCD(struct LCD_Setup LCD_cfg, int lineNo, int colNo, char *str)
{
	lcd_put_cur(LCD_cfg, lineNo, colNo);
	lcd_send_string(LCD_cfg, "				"); // Erases the existing content fully

	lcd_put_cur(LCD_cfg, lineNo, colNo);
	lcd_send_string(LCD_cfg, str);
}

void write_hex_on_LCD(struct LCD_Setup LCD_cfg, int lineNo, int colNo, uint8_t hex)
{
	char buffer[16];
	sprintf(buffer, "0x%02X", hex); // display hexadecimal
	
	lcd_put_cur(LCD_cfg, lineNo, colNo);
	lcd_send_string(LCD_cfg, buffer);
}


void LCD_Queue_Receiver_Task(void *params)
{

	struct LCD_Setup * LCD_cfg_ptr = (struct LCD_Setup *)params;
	struct LCD_Setup LCD_cfg = * LCD_cfg_ptr;

	QueueHandle_t myQueue = LCD_cfg.keyQueue;
	ESP_LOGI(TAG, "Now in LCD queue receiver task");
	
	UBaseType_t uxHighWaterMark; // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark

	while (true)
	{
	    /* Inspect our own high water mark on entering the task. */
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark

		char keyPressed = 0; // This is the real statement
		xQueueReceive(myQueue, &keyPressed, ( TickType_t ) 0); // take the key found in the queue
		if (keyPressed != 0) // if a key is found
		{
			char buffer[16];
			sprintf(buffer, "Last pressed %c", keyPressed); // display hexadecimal  // This is the real statement
			write_string_on_LCD(LCD_cfg, 1, 0, buffer); // display it on line 2 of the LCD though as string
			ESP_LOGI(TAG, "Wrote keyPressed on LCD 2nd line. Last pressed %c", keyPressed);
			ESP_LOGI(TAG, "LCD_Queue_Receiver_Task uxHighWaterMark = %u", uxHighWaterMark); // Refer - https://www.freertos.org/FreeRTOS_Support_Forum_Archive/July_2019/freertos_Understanding_uxTaskGetStackHighWaterMark_results_51c44e8598j.html
		}
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}

void LCD_Counter_Task(void *params)
{
	// This tasks autonomously displays elapsed time as a counter on the top line of the LCD. It does not depend on any queue, interrupts or triggers
	struct LCD_Setup * LCD_cfg_ptr = (struct LCD_Setup *)params;
	struct LCD_Setup LCD_cfg = * LCD_cfg_ptr;

	ESP_LOGI(TAG, "Now in LCD_Counter_Task task");
	char elapsed_count = 1; 
	UBaseType_t uxHighWaterMark; // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark

	while (true)
	{
	    /* Inspect our own high water mark on entering the task. */
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark

		char buffer[16];
		sprintf(buffer, "Counter %d   ", elapsed_count++);
		write_string_on_LCD(LCD_cfg, 0, 0, buffer); // display it on line 1 of the LCD though as string
		ESP_LOGI(TAG, "Wrote counter on LCD 1st line");
		vTaskDelay( COUNTER_MS / portTICK_PERIOD_MS); // Delay for COUNTER_MS and count next. Since vTaskDelay takes only xTicksToDelay as argument, it has to be divided by portTICK_PERIOD_MS which is the number of milliseconds a scheduler TICK takes
		ESP_LOGI(TAG, "LCD_Counter_Task uxHighWaterMark = %u", uxHighWaterMark); // Refer - https://www.freertos.org/FreeRTOS_Support_Forum_Archive/July_2019/freertos_Understanding_uxTaskGetStackHighWaterMark_results_51c44e8598j.html
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}
