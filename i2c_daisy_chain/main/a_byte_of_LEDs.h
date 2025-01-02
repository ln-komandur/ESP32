/*
 * a_byte_of_LEDs.h
 *
 *  Created on: Jan 1, 2025
 *      Author: ln-komandur
 */

#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <esp_timer.h>

void init_LED_blink(i2c_port_t i2c_port_num, uint8_t startByte, long duration);

extern long blinkDuration; // in microseconds

extern i2c_port_t i2c_port_num; 

extern esp_err_t err;
