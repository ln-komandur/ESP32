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


static bool isKeyBeingRead = false;

// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
QueueHandle_t interruptQueue;

/* Inside the interrupt service routine we will call the xQueueSendFromISR() function.
 * It will send an item from a queue. This function takes in 3 parameters.
 * The 1st is ‘xQueue’ which is the queue handle on which the item is to be posted.
 * In our case it is ‘interuptQueue.’
 * The second parameter is the ‘pvItemToQueue’ which is the pointer to the item that is placed on the queue.
 * In our case it is ‘&pinNumber.’ The third parameter is the ‘pxHigherPriorityTaskWoken’ which is NULL in our case.
 */
static void IRAM_ATTR gpio_interrupt_handler(void *args)
{
	if (!isKeyBeingRead) // if a key is not being read, then add an item to the queue
	{
		int pinNumber = (int)args;
		xQueueSendFromISR(interruptQueue, &pinNumber, NULL);
		// Don't use ESP_LOGx from ISRs. If you must, you can use ESP_EARLY_LOG* instead, but it's preferred to not log in ISRs at all as you'd want those to last as short as possible. It will reboot with "lock_acquire_generic at /home/meander/esp-idf-v5.3.2/components/newlib/locks.c:133"
	}
	// Don't use ESP_LOGx from ISRs. If you must, you can use ESP_EARLY_LOG* instead, but it's preferred to not log in ISRs at all as you'd want those to last as short as possible. It will reboot with "lock_acquire_generic at /home/meander/esp-idf-v5.3.2/components/newlib/locks.c:133"

}

uint8_t get_keypad_pins(struct passive_Matrix_keyPad_Setup kpdCfg) 
{
	//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
	//  Reading from read_from_PCF8574_pins
	uint8_t rx_data[1];
	
	// This happens in the IO expander when reading data using it
	// Receive / Read from the device
	err = i2c_master_receive(kpdCfg.device_handle, rx_data, 1, TIMEOUT_MS); // -1 for timeout means wait forever.

	if (err!=0)	
		ESP_LOGI(TAG, "Error in getting data from PCF8574 keypad");
	else
		ESP_LOGI(TAG, "get_keypad_pins successful. About to return byte found");

	return rx_data[0]; // there is only one element in this array. Return it.
}

void set_keypad_pins(struct passive_Matrix_keyPad_Setup kpdCfg, uint8_t data)
{
	i2c_master_dev_handle_t device_handle = kpdCfg.device_handle;
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	
	// Transmit / Write to the device
	err = i2c_master_transmit(device_handle, data_t, len, TIMEOUT_MS); // -1 means wait forever. This call returns ESP_ERR_INVALID_STATE.

	if (err!=0)	
		ESP_LOGI(TAG, "Error in sending data to PCF8574 keypad");
	else
		ESP_LOGI(TAG, "set_keypad_pins successful");

}

void find_key(struct passive_Matrix_keyPad_Setup kpdCfg)
{
	uint8_t byteOfColumns; //  used for storing what is read from PCF8574 keypad
	uint8_t bitSetOnColumn = 0;
	//uint8_t bufferToLogger[1]; 
	int col = 0;
	ESP_LOGI(TAG, "find_key() ; byteOfColumns = get_keypad_pins()");
	byteOfColumns = get_keypad_pins(kpdCfg);
	ESP_LOGI(TAG, "found the column: byteOfColumns by get_keypad_pins(kpdCfg) ");
	
	// find the column
	bitSetOnColumn = (255 - byteOfColumns)&0xf0;
	ESP_LOGI(TAG, "find_key() ;  bitSetOnColumn");

	// set that column to 1 and others to 0
	switch (bitSetOnColumn) {
		case 128: 	// bitSetOnColumn = 0x80: 10000000. byteOfColumns = 01110000 = 112 = 0x70
			col = 1; break;
		case 64: 	// bitSetOnColumn = 0x40: 01000000. byteOfColumns = 10110000 = 176 = 0xb0
			col = 2; break;
		case 32: 	// bitSetOnColumn = 0x20: 00100000. byteOfColumns = 11010000 = 208 = 0xd0
			col = 3; break;
		case 16: 	// bitSetOnColumn = 0x10: 00010000. byteOfColumns = 11100000 = 224 = 0xe0
			col = 4; break;
	}
	ESP_LOGI(TAG, "set_keypad_pins(0x0f) - set all row pins high now");
	
	set_keypad_pins(kpdCfg, 0x0f); // set all row pins high now
	ESP_LOGI(TAG, "find the row");

	// find the row
	uint8_t byteOfRows; //  used for storing what is read from PCF8574 keypad
	ESP_LOGI(TAG, "byteOfRows = get_keypad_pins()");
	byteOfRows = get_keypad_pins(kpdCfg);

	uint8_t bitSetOnRow;
	bitSetOnRow = (15 - byteOfRows)&0x0f;
	char keyPressed = 0;
	switch (bitSetOnRow) {
		case 8: 	// bitSetOnRow = 0x08: 00001000. byteOfRows = 00000111 = 7 = 0x07
			keyPressed = keys[col-1][0];break;
		case 4: 	// bitSetOnRow = 0x04: 00000100. byteOfRows = 00001011 = 11 = 0x0b
			keyPressed = keys[col-1][1];break;
		case 2: 	// bitSetOnRow = 0x02: 00000010. byteOfRows = 00001101 = 13 = 0x0d
			keyPressed = keys[col-1][2];break;
		case 1: 	// bitSetOnRow = 0x01: 00000001. byteOfRows = 00001110 = 14 = 0x0e
			keyPressed = keys[col-1][3];break;
	}
	ESP_LOGI(TAG, "fOUNd the row, and then xQueueSend to keyQueue");

	xQueueSend(kpdCfg.keyQueue, &keyPressed, ( TickType_t ) 0); // after finding the key, put it in the keyqueue so that the LCD can pick it up
	ESP_LOGI(TAG, "set_keypad_pins(0xf0) -> write 11110000 and re-initialize the keypad");
	set_keypad_pins(kpdCfg, 0xf0); // write 11110000 and re-initialize the keypad as both the column and the row are found by now
}



/* Inside this function, we first create two integer variables ‘pinNumber’ and ‘count’ and
 * set their value to 0. Then inside the infinite while function, we check if an item is
 * received from the queue by using the xQueueReceive() function. This function takes in
 * three parameters. The first parameter is the ‘xQueue’ which is the queue handle from which
 * the item is to be received. It is ‘interuptQueue’ in our case. The second parameter is the
 * ‘pvBuffer’ which is the pointer to the buffer into which the received item will be copied.
 * In our case it is ‘&pinNumber.’ The third parameter is the ‘xTicksToWait’ which is the
 * maximum amount of time the task should block waiting for an item to receive should the
 * queue be empty at the time of the call. In our case it is set as ‘portMAX_DELAY.’
 * Thus, when the interrupt occurs then gpio_set_level() function is called to set the level of
 * the LED_PIN according to the level of the INTERRUPT_PIN.
 * At this time the ESP-IDF terminal prints the message that the GPIO was pressed,
 * its state, the pin Number and count.
 * The above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 * It also calls the showKeyPressed() function to display the key
 */


void Key_Ctrl_Task(void *params) // this function will be called when interrupt happens.
{
	struct passive_Matrix_keyPad_Setup * kpd_cfg_ptr = (struct passive_Matrix_keyPad_Setup *)params;
	struct passive_Matrix_keyPad_Setup kpd_cfg = * kpd_cfg_ptr;

	int pinNumber = 0;
	int count = 0;
	
	ESP_LOGI(TAG, "Key_Ctrl_Task(void *params) ");


	while (true)
	{
		ESP_LOGI(TAG, "WAITING: IF xQueueReceive(interruptQueue ... has something in it");
		if (xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY))
		{
			ESP_LOGI(TAG, "THEN do pinState = gpio_get_level(kpd_cfg.interruptPin)");
			int pinState = gpio_get_level(kpd_cfg.interruptPin);
			ESP_LOGI(TAG, "GOT  pinState");
			ESP_LOGI(TAG, "GPIO%d Pin state is %d. isKeyBeingRead is %d\n", pinNumber, pinState, isKeyBeingRead);

			if (pinState == 0) // only when pin goes down (Falling edge)
			{
				isKeyBeingRead = true; // block more items from being added to the queue until this key is found
				printf("Key Released. Pin gone down (Falling edge). isKeyBeingRead is set to true = %d\n", isKeyBeingRead);
				ESP_LOGI(TAG, "Key Released on PCF8574 keypad. Pin gone down (Falling edge). isKeyBeingRead is set to true");
				find_key(kpd_cfg);
				isKeyBeingRead = false; // reset flag so that the next key press can be added to the queue
				ESP_LOGI(TAG, "GPIO%d was pressed %d times. Pin state is %d. isKeyBeingRead is released %d\n", pinNumber, ++count, pinState, isKeyBeingRead);
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
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}


void init_keypad(struct passive_Matrix_keyPad_Setup kpd_cfg) 
{
	set_keypad_pins(kpd_cfg, 0xf0); // write 11110000
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
	
	xTaskCreate(Key_Ctrl_Task, "Key_Ctrl_Task", 2048, &kpd_cfg, 1, NULL);
	ESP_LOGI(TAG, "xTaskCreate(Key_Ctrl_Task,2048,&kpd_cfg,1,NULL)");
	/* Install the interrupt service routine with the flags for interrupt allocation
	 * Add the ISR handler for the INTERRUPT_PIN using gpio_isr_handler_add()
	 */
	gpio_install_isr_service(0);
	ESP_LOGI(TAG, "gpio_install_isr_service");

	gpio_isr_handler_add(kpd_cfg.interruptPin, gpio_interrupt_handler, (void *)kpd_cfg.interruptPin);
	ESP_LOGI(TAG, "gpio_isr_handler_add");

	// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

	// kpd_cfg.keyQueue should not be created here with  = xQueueCreate(32, sizeof(char)); A common queue is being passed to the kpd_cfg to be shared with the LCD for reading it

}
