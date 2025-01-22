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

#ifndef HEADER_i2c_LCD_H_ // Include guard to prevent multiple inclusion / conditional compilation
#define HEADER_i2c_LCD_H_


i2c_master_dev_handle_t get_i2c_device_handle(i2c_master_bus_handle_t bus_handle, uint8_t device_address); // get the device handle

void lcd_init (i2c_master_dev_handle_t LCD_dev_handle);   // initialize lcd at the device handle

void lcd_send_cmd (i2c_master_dev_handle_t LCD_dev_handle, char cmd);  // send command to the lcd

void lcd_send_data (i2c_master_dev_handle_t LCD_dev_handle, char data);  // send data to the lcd

void lcd_send_string (i2c_master_dev_handle_t LCD_dev_handle, char *str);  // send string to the lcd

void lcd_put_cur(i2c_master_dev_handle_t LCD_dev_handle,int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (i2c_master_dev_handle_t LCD_dev_handle); // Clears the LCD

void write_string_on_LCD(i2c_master_dev_handle_t LCD_dev_handle, int lineNo, int colNo, char *str); // Writes a string onto the LCD

void write_hex_on_LCD(i2c_master_dev_handle_t LCD_dev_handle, int lineNo, int colNo, uint8_t hex); // Writes a HEX onto the LCD

#endif //HEADER
