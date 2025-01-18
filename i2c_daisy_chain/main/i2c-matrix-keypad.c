/*
 * i2c_matrix-keypad.h
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * Good reference 
 * - https://www.makeriot2020.com/index.php/2020/10/05/using-i2c-with-a-4x4-matrix-keypad/
 * - https://embeddedexplorer.com/esp32-i2c-tutorial/
 * - https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 */


#include <i2c-matrix-keypad.h>
#include <stdint.h>


#define TIMEOUT_MS		1000

static const char *TAG = "i2c_Matrix-Keypad";

// Set the Key at Use (4x4)
char keys [4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
}; // Reference - https://www.makeriot2020.com/index.php/2020/10/05/using-i2c-with-a-4x4-matrix-keypad/


// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
QueueHandle_t interruptQueue;

/* Inside the interrupt service routine we will call the xQueueSendFromISR() function.
 * It will send an item from a queue. This function takes in 3 parameters.
 * The 1st is ‘xQueue’ which is the queue handle on which the item is to be posted.
 * In our case it is ‘interuptQueue.’
 * The second parameter is the ‘pvItemToQueue’ which is the pointer to the item that is placed on the queue.
 * In our case it is ‘&intrpt_Q_Val.’ The third parameter is the ‘pxHigherPriorityTaskWoken’ which is NULL in our case.
 */
static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
	int intrpt_Q_Val = (int)args; // We can put value you in this. We have a dummy value
	xQueueSendFromISR(interruptQueue, &intrpt_Q_Val, NULL);
	// Don't use ESP_LOGx from ISRs. If you must, you can use ESP_EARLY_LOG* instead, but it's preferred to not log in ISRs at all as you'd want those to last as short as possible. It will reboot with "lock_acquire_generic at /home/meander/esp-idf-v5.3.2/components/newlib/locks.c:133"
}

uint8_t get_keypad_pins(struct passive_Matrix_keyPad_Setup kpdCfg) 
{
	//  Reading from read_from_PCF8574_pins . Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
	uint8_t rx_data[1];
	
	// Receive / Read from the device. This happens in the IO expander
	err = i2c_master_receive(kpdCfg.device_handle, rx_data, 1, TIMEOUT_MS); // -1 for timeout means wait forever.

	if (err!=0)	
		ESP_LOGI(TAG, "Error in getting data from PCF8574 keypad");
	else
		ESP_LOGI(TAG, "get_keypad_pins successful. About to return byte found");

	return rx_data[0]; // there is only one element in this array. Return it.
}


/* Inside this function, we first create an integer variable ‘intrpt_Q_Val’ and set its value
 * to 0. Then inside the infinite while function, we check if an item is
 * received from the queue by using the xQueueReceive() function. This function takes in
 * three parameters. The first parameter is the ‘xQueue’ which is the queue handle from which
 * the item is to be received. It is ‘interuptQueue’ in our case. The second parameter is the
 * ‘pvBuffer’ which is the pointer to the buffer into which the received item will be copied.
 * In our case it is ‘&intrpt_Q_Val.’ The third parameter is the ‘xTicksToWait’ which is the
 * maximum amount of time the task should block waiting for an item to receive should the
 * queue be empty at the time of the call. In our case it is set as ‘portMAX_DELAY.’
 * Thus, when the interrupt occurs then gpio_set_level() function is called to set the level of
 * the LED_PIN according to the level of the INTERRUPT_PIN.
 * At this time the ESP-IDF terminal prints the message that the GPIO was pressed.
 * The above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 * It also calls the showKeyPressed() function to display the key
 */


void Key_Ctrl_Task(void *params) // this function will be called when interrupt happens.
{
	struct passive_Matrix_keyPad_Setup kpd_cfg = * (struct passive_Matrix_keyPad_Setup *)params;

	int intrpt_Q_Val = 0; // We can put value you in this. We have a dummy value
	
	ESP_LOGI(TAG, "Key_Ctrl_Task(void *params) ");
	UBaseType_t uxHighWaterMark; // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark

	while (true)
	{
	    /* Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark 
	    * Optionally inspect our own high water mark on entering the task.
	    */
        // uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark
        
		ESP_LOGI(TAG, "WAITING: IF xQueueReceive(interruptQueue ... has something in it");
		if (xQueueReceive(interruptQueue, &intrpt_Q_Val, portMAX_DELAY))
		{
			ESP_LOGI(TAG, "THEN do pinState = gpio_get_level(kpd_cfg.interruptPin)");

			if (gpio_get_level(kpd_cfg.interruptPin) == 0) // only when pin goes down (Falling edge)
			{
				ESP_LOGI(TAG, "Key Released on PCF8574 keypad. Pin gone down (Falling edge). Finding column from 1 to 4");

				int col = 0;

				// set that column to 1 and others to 0
				switch ((255 - get_keypad_pins(kpd_cfg))&0xf0) {
					case 128: 	// bitSetOnColumn = 0x80: 10000000. byteOfColumns = 01110000 = 112 = 0x70
						col = 1; break;
					case 64: 	// bitSetOnColumn = 0x40: 01000000. byteOfColumns = 10110000 = 176 = 0xb0
						col = 2; break;
					case 32: 	// bitSetOnColumn = 0x20: 00100000. byteOfColumns = 11010000 = 208 = 0xd0
						col = 3; break;
					case 16: 	// bitSetOnColumn = 0x10: 00010000. byteOfColumns = 11100000 = 224 = 0xe0
						col = 4; break;
				}
				ESP_LOGI(TAG, "set_keypad_pins(0x0f) - set all row pins high now with 0x0f");
				
				// set all row pins high now
				err = i2c_master_transmit(kpd_cfg.device_handle, (uint8_t []){0x0f}, 1, TIMEOUT_MS); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.

				ESP_LOGI(TAG, "finding the row");
				switch ((15 - get_keypad_pins(kpd_cfg))&0x0f) { // 15 is 00001111
					case 8: 	// bitSetOnRow = 0x08: 00001000. byteOfRows = 00000111 = 7 = 0x07
						xQueueSend(kpd_cfg.keyQueue, &keys[col-1][0], ( TickType_t ) 0); // after finding the key, put it in the keyqueue so that the LCD can pick it up
						break;
					case 4: 	// bitSetOnRow = 0x04: 00000100. byteOfRows = 00001011 = 11 = 0x0b
						xQueueSend(kpd_cfg.keyQueue, &keys[col-1][1], ( TickType_t ) 0); // after finding the key, put it in the keyqueue so that the LCD can pick it up				
						break;
					case 2: 	// bitSetOnRow = 0x02: 00000010. byteOfRows = 00001101 = 13 = 0x0d
						xQueueSend(kpd_cfg.keyQueue, &keys[col-1][2], ( TickType_t ) 0); // after finding the key, put it in the keyqueue so that the LCD can pick it up
						break;
					case 1: 	// bitSetOnRow = 0x01: 00000001. byteOfRows = 00001110 = 14 = 0x0e
						xQueueSend(kpd_cfg.keyQueue, &keys[col-1][3], ( TickType_t ) 0); // after finding the key, put it in the keyqueue so that the LCD can pick it up	
						break;
				}
				ESP_LOGI(TAG, "FOUND the row, and CALLED xQueueSend and SEND to keyQueue set_keypad_pins(0xf0) -> write 11110000 and re-initialize the keypad");
				// write 11110000 and re-initialize the keypad as both the column and the row are found by now
				// Transmit / Write to the device
				err = i2c_master_transmit(kpd_cfg.device_handle, (uint8_t []){0xf0}, 1, TIMEOUT_MS); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.

				ESP_LOGI(TAG, "GPIO%d was pressed", intrpt_Q_Val);
			}
			else
			{
				ESP_LOGI(TAG, "Key is Pressed but not yet released. Discarding the interrupt and will wait for the next interrupt on key release");
			}
		}
		else
		{
			ESP_LOGI(TAG, "Key NOT Pressed on PCF8574 keypad ");
		}
		/* 
		Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark
		Calling the function will have used some stack space, we would therefore now expect uxTaskGetStackHighWaterMark() to return a 
       	value lower than when it was called on entering the task. 
       	
       	A task may query its own high water mark by passing NULL as the xTask parameter for the handle of the task being queried.
       	*/
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Refer - https://www.freertos.org/FreeRTOS_Support_Forum_Archive/July_2019/freertos_Understanding_uxTaskGetStackHighWaterMark_results_51c44e8598j.html
		
		ESP_LOGI(TAG, "Key_Ctrl_Task uxHighWaterMark = %u", uxHighWaterMark); 

	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}


void init_keypad(struct passive_Matrix_keyPad_Setup kpd_cfg) 
{
	// Initialize 11110000 on the keypad pins to detect the columns
	err = i2c_master_transmit(kpd_cfg.device_handle, (uint8_t []){0xf0}, 1, TIMEOUT_MS); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.
	ESP_LOGI(TAG, "set_keypad_pins to 0xf0 = 11110000");

	// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
	/*
	 * This way when the push button will be pressed, the state of the INTERRUPT_PIN will
	 * go HIGH and when it will be released then the state of the INTERRUPT_PIN will go LOW.
	 *
	 */
	// We configure the INTERRUPT_PIN as a GPIO and then set its direction as an input.
	esp_rom_gpio_pad_select_gpio(kpd_cfg.interruptPin);
	ESP_LOGI(TAG, "esp_rom_gpio_pad_select_gpio");

	gpio_set_direction(kpd_cfg.interruptPin, GPIO_MODE_INPUT);
	ESP_LOGI(TAG, "gpio_set_direction");


	/* We enable the pullup resistor and disable the pulldown resistor on the interrupt pin.
	 * https://www.best-microcontroller-projects.com/pcf8574.html says enable pull-up
	 * https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
	 * The interrupt open-drain output pin is active LOW.  It is normally pulled HIGH
	 * using a pull-up resistor and is driven low by the PCF8574 when any of the inputs
	 * change state. This signals the MCU to poll the part to see what is going on.
	 * If connecting this pin, enable the internal pull-up resistor on the MCU or add
	 * an external pull-up of 10K or so.
	 */

	gpio_pullup_en(kpd_cfg.interruptPin);
	ESP_LOGI(TAG, "gpio_pullup_en");

	gpio_pulldown_dis(kpd_cfg.interruptPin);
	ESP_LOGI(TAG, "gpio_pulldown_dis");

	/* Then, we will set the interrupt trigger type on the INTERRUPT_PIN using gpio_set_intr_type() function.
	 * This function takes in two parameters.
	 * The first parameter is the GPIO pin number which is ‘INTERRUPT_PIN’ in our case.
	 * The second parameter is the trigger type which is set as ‘GPIO_INTR_NEGEDGE’ (falling)
	 * which means the interrupt will be triggered when the the state of the pin changes from HIGH to LOW.
	 * We use 'GPIO_INTR_NEGEDGE' because, as soon as an interrupt is triggered (i.e. falling edge is detected)
	 * we need the rows and columns of the key pad to remain connected so that we can apply masking bytes on 
	 * rows and columns and find their connection (intersection) to then find the key pressed. 
	 * If we had used ''GPIO_INTR_POSEDGE', when we get around to reading the interrupt, the condition that 
	 * caused the interrupt would have gone, we won't know where the rows and columns of the key pad were 
	 * connected and there's no way to find out the key pressed.
	 * Refer falling edge per https://www.best-microcontroller-projects.com/pcf8574.html
	 */
	
	gpio_set_intr_type(kpd_cfg.interruptPin, GPIO_INTR_NEGEDGE);
	ESP_LOGI(TAG, "gpio_set_intr_type");

	/* Create a queue that can hold 10 elements, each of size int.
	 * Then create a task using xTaskCreate with necessary arguments. Refer https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
	 */
	interruptQueue = xQueueCreate(10, sizeof(int));
	ESP_LOGI(TAG, "interruptQueue = xQueueCreate(10, sizeof(int))");
	
	xTaskCreate(&Key_Ctrl_Task, "Key_Ctrl_Task", 2560, &kpd_cfg, 1, NULL);
	ESP_LOGI(TAG, "xTaskCreate(Key_Ctrl_Task,2560,&kpd_cfg,1,NULL)");
	/* Install the interrupt service routine with the flags for interrupt allocation
	 * Add the ISR handler for the INTERRUPT_PIN using gpio_isr_handler_add()
	 */
	gpio_install_isr_service(0);
	ESP_LOGI(TAG, "gpio_install_isr_service");
	
	int dummy_Val_For_intrpt_Q = 1;
	gpio_isr_handler_add(kpd_cfg.interruptPin, gpio_interrupt_handler, (void *)dummy_Val_For_intrpt_Q);
	ESP_LOGI(TAG, "gpio_isr_handler_add");

	// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

	// kpd_cfg.keyQueue should not be created here with  = xQueueCreate(32, sizeof(char)); A common queue is being passed to the kpd_cfg to be shared with the LCD for reading it

}
