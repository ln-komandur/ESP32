/*
 * PCF8574_device.cpp
 *
 *  Created on: Jan 4, 2025
 *      Author: Ln-Komandur
 */

#include <PCF8574_device.hpp>

static PCF8574_IO_Expander connectedExpander;
static long blinkDuration;
static uint8_t byteToBlinkLEDs;

static const uint16_t taskStackDepth = 1024;
static const char* taskName = "LED Blinker";
UBaseType_t taskPriority = 1;
TaskHandle_t* blinkTaskHandle;
    
PCF8574_device::PCF8574_device(i2c_master_bus_handle_t bus_handle, uint8_t jumper_address, long duration, uint8_t blinkByte)
{
	connectedExpander = PCF8574_IO_Expander(bus_handle,jumper_address);
	blinkDuration = duration;
	byteToBlinkLEDs = blinkByte;
}

/*
 * Initialize LED blinking task
*/
void PCF8574_device::start_blinking() 
{
    xTaskCreate(blink_LED_Task, taskName, taskStackDepth, NULL, taskPriority, blinkTaskHandle);

}



void PCF8574_device::blink_LED_Task(void *params)
{
	int64_t lastBlinkTime; // When was the last blink
	lastBlinkTime = esp_timer_get_time();
	uint8_t aByte = byteToBlinkLEDs;
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
			connectedExpander.write_byte_to_pins(aByte);
			aByte = aByte << 1;
			if (aByte == 0x00) {
				aByte = byteToBlinkLEDs;
			}
			lastBlinkTime = callTime;
		}
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}
