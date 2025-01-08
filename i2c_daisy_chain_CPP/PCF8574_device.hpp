/*
 * PCF8574_device.hpp
 *
 *  Created on: Jan 4, 2025
 *      Author: Ln-Komandur
 */

#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>


#include <esp_log.h>
#include <esp_err.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/i2c_master.h>
#include <unistd.h>
#include "PCF8574_IO_Expander.hpp"

#ifndef MAIN_PCF8574_DEVICE_H_
#define MAIN_PCF8574_DEVICE_H_

class PCF8574_device {

private:
    static void blink_LED_Task(void *params); // Applies only for LEDs connected to a PCF8574_DEVICE


public:
	PCF8574_device(i2c_master_bus_handle_t bus_handle, uint8_t jumper_address, long duration, uint8_t blinkByte);
	/*
	 * Initialize LED blinking task
	*/
    void start_blinking();
	
};
#endif // MAIN_PCF8574_DEVICE_H_
