/*
 * a_byte_of_LEDs.h
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * Creates a task to light up LEDs connected via the i2c bus with a PCF8574 IO expander.
 * LEDs light up on each pin of the PCF8574 based on bits written to them as a byte.
 * LEDs also blink by shifting that byte left and reseting to the initial byte when all are off (0x00).
 * The address of the PCF8574 IO expander is handled in the calling functions. 
 * The functions below only get a device handle which is obtained by the calling function using a bus handle.
 */
 
#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c_master.h>
#include <esp_timer.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/*
 * Initialize LED blinking task
 * Creates a task with a 
 *		* device handle
 *		* blinkDuration
 *		* startByte 
 */
void init_LED_blink(i2c_master_dev_handle_t device_handle, uint8_t startByte, long duration);

/* Lights up a byte on the 8 LEDs connected to a PCF8574
 * Takes the device handle, and the byte to be lit up as inputs
 */
void show_byte_with_LEDs(i2c_master_dev_handle_t device_handle , uint8_t aByte);



/* Creates a task that blinks the 8 LEDs connected to a PCF8574. 
 * e.g. Starts with a byte (e.g. 0x55) and shifts it left after every `blinkDuration`. 
 * When the byte written is 0x00, it is reset to the start byte (0x55) again.
 */
void blink_LEDs_Task(void *params);

extern long blinkDuration; // in microseconds

extern i2c_port_t i2c_port_num; 

extern esp_err_t err;
