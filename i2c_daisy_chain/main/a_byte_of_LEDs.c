/*
 * a_byte_of_LEDs.c
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * Creates a task to light up LEDs connected via the i2c bus with a PCF8574 IO expander.
 * LEDs light up on each pin of the PCF8574 based on bits written to them as a byte.
 * LEDs also blink by shifting that byte left and reseting to the initial byte when all are off (0x00).
 * The address of the PCF8574 IO expander is handled in the calling functions. 
 * The functions below only get a device handle which is obtained by the calling function using a bus handle.
 */


#include <a_byte_of_LEDs.h>
#include <PCF8574_IO_Expander.h>

static const char *TAG = "A_BYTE_ON_LEDs";

uint8_t startByte; // This is the byte that is initialized

int64_t lastBlinkTime; // When was the last blink

/* Lights up a byte on the 8 LEDs connected to a PCF8574
 * Takes the device handle, and the byte to be light up as inputs
 */

void show_byte_with_LEDs(i2c_master_dev_handle_t device_handle , uint8_t aByte)
{
	write_byte_to_pins(device_handle ,aByte);
	ESP_LOGI(TAG, "show_byte_with_LEDs");
}

/* Creates a task that blinks the 8 LEDs connected to a PCF8574. 
 * e.g. Starts with a byte (e.g. 0x55) and shifts it left after every `blinkDuration`. 
 * When the byte written is 0x00, it is reset to the start byte (0x55) again.
 */


void blink_LEDs_Task(void *params)
{
	lastBlinkTime = esp_timer_get_time();
	uint8_t aByte = startByte;
	i2c_master_dev_handle_t device_handle = *(i2c_master_dev_handle_t *)params;

	while (true)
	{
	    int64_t callTime = esp_timer_get_time();

		/* Example:
		 * startByte = 0x55 = 01010101, which,
		 * when shifted left once gives 0xaa = 10101010, which
		 * when shifted left once gives 0x54 = 01010100, which
		 * when shifted left once gives 0xa8 = 10101000, which
		 * when shifted left once gives 0x50 = 01010000, which
		 * when shifted left once gives 0xa0 = 10100000, which
		 * when shifted left once gives 0x40 = 01000000, which
		 * when shifted left once gives 0x80 = 10000000, which
		 * when shifted left once gives 0x00 = 00000000, when the byte is reset to startByte
		 */
		if (callTime - lastBlinkTime > blinkDuration)
		{
			write_byte_to_pins(device_handle, aByte);
			//ESP_LOGI(TAG, "blinking LEDs by writing to pin");

			aByte = aByte << 1;
			if (aByte == 0x00) {
				aByte = startByte;
			}
			lastBlinkTime = callTime; 
		}
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}

/*
 * Initialize LED blinking task
 * Creates a task with a 
 *		* device handle
 *		* blinkDuration
 *		* startByte 
*/
void init_LED_blink(i2c_master_dev_handle_t device_handle, uint8_t initByte, long duration)
{
	blinkDuration = duration;
	startByte = initByte;
	xTaskCreate(blink_LEDs_Task, "blink_LEDs_Task", 2048, &device_handle, 1, NULL);
	ESP_LOGI(TAG, "Created blink LEDs task");

}
