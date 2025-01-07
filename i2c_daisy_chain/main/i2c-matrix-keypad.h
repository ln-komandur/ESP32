/*
 * i2c_matrix-keypad.h
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * The LCD 1602A is based on the Hitachi HD44780 LCD controller.
 * Good reference 
 * - https://www.makeriot2020.com/index.php/2020/10/05/using-i2c-with-a-4x4-matrix-keypad/
 * - https://embeddedexplorer.com/esp32-i2c-tutorial/
 * - https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 */
 

#include <esp_err.h>
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <driver/gpio.h>

#ifndef HEADER_i2c_matrix_keypad_H_ // Include guard to prevent multiple inclusion
#define HEADER_i2c_matrix_keypad_H_

extern esp_err_t err;

struct passive_Matrix_keyPad_Setup {
	i2c_master_dev_handle_t device_handle;
	int interruptPin;
	QueueHandle_t keyQueue; // used for storing what is read from PCF8574 keypad
};

void init_keypad(struct passive_Matrix_keyPad_Setup kpd_Config); // this will assign the i2c port, and initialize data pins as well as interrupt pin
uint8_t get_keypad_pins(struct passive_Matrix_keyPad_Setup kpdCfg);
void set_keypad_pins(struct passive_Matrix_keyPad_Setup kpdCfg, uint8_t data);
void find_key(struct passive_Matrix_keyPad_Setup kpdCfg);
void Key_Ctrl_Task(void *params); // this function will be called when interrupt happens.

#endif
