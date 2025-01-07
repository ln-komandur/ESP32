/*
 * i2c_LCD.h
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

#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <unistd.h>


struct LCD_Setup {
	i2c_master_dev_handle_t device_handle; // holds the device handle to the LCD
	QueueHandle_t keyQueue; // this queue holds the keys pressed. Its size as 32, is the total number of characters on a 1602 LCD display
};

void lcd_init (struct LCD_Setup LCD_cfg);   // initialize lcd at the device handle

void lcd_send_cmd (struct LCD_Setup LCD_cfg, char cmd);  // send command to the lcd

void lcd_send_data (struct LCD_Setup LCD_cfg, char data);  // send data to the lcd

void lcd_send_string (struct LCD_Setup LCD_cfg, char *str);  // send string to the lcd

void lcd_put_cur(struct LCD_Setup LCD_cfg,int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (struct LCD_Setup LCD_cfg); // Clears the LCD

void write_string_on_LCD(struct LCD_Setup LCD_cfg, int lineNo, int colNo, char *str); // Writes a string onto the LCD

void write_hex_on_LCD(struct LCD_Setup LCD_cfg, int lineNo, int colNo, uint8_t hex); // Writes a HEX onto the LCD

void LCD_Queue_Receiver_Task(void *params); // A task that pools a queue to find keys and display on the 2nd line of the LCD

void LCD_Counter_Task(void *params); // An independent task that display a counter on the 1st line of the LCD

