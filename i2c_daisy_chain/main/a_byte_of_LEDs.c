/*
 * a_byte_of_LEDs.c
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * Creates tasks to light up LEDs connected via tha i2c bus with a PCF8574 expander.
 * LEDs light up on each pin of the PCF8574 based on bits written to them as a byte.
 * LEDs can also blink by shifting that byte left and reseting to the initial byte when all are off.
 */



#include <a_byte_of_LEDs.h>
#include <PCF8574.h>

#define PCF8574_SLAVE_ADDR 0x21 // A0 jumper to ground

#define TIMEOUT_MS		1000 // milliseconds


static const char *TAG = "BYTE_ON_LEDs";

uint8_t startByte; // This is the byte that is initialized

int64_t lastBlinkTime; // When was the last blink

/* Lights up a byte on the 8 LEDs connected to a PCF8574
 * Takes the byte to be lit up as the input
 */

void show_byte_with_LEDs(uint8_t aByte)
{
	write_byte_to_pins(i2c_port_num, aByte);
	ESP_LOGI(TAG, "show_byte_with_LEDs");
}

/* Creates a task that blinks the 8 LEDs connected to a PCF8574. 
 * e.g. Starts with 0x55 as a byte and shifts it left after every `blinkDuration`. When the byte written is 0x00, it is reset to 0x55 again
 */


void blink_LEDs_Task(void *params)
{
	lastBlinkTime = esp_timer_get_time();
	uint8_t aByte = startByte;

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
			write_byte_to_pins(i2c_port_num, aByte);
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
*/
void init_LED_blink(i2c_port_t port_num, uint8_t initByte, long duration)
{
	i2c_port_num = port_num;
	xTaskCreate(blink_LEDs_Task, "blink_LEDs_Task", 2048, NULL, 1, NULL);
	blinkDuration = duration;
	startByte = initByte;

}
