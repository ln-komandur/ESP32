// Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/

#include <stdio.h>
#include "i2c-matrix-keypad.h"
#include "esp_log.h"
#include "driver/i2c.h"


// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"

/* The interrupt pin 'INT' from PCF8574 is connected to GPIO15 of ESP32.
 * ‘INPUT_PIN’ is used to read the digital input from pin number 15.
 */

#define INPUT_PIN 15

// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/


#define PCF8574_SLAVE_KPD_ADDRESS 0x20 // No jumper set for keypad on PCF8574 expander

#define TIMEOUT_MS		1000
// #define DELAY_MS		1000

esp_err_t err;

static const char *TAG = "Matrix Keypad";

// Set the Key at Use (4x4)
char keys [4][4] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

static bool isKeyBeingRead = false;

// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
int state = 0;
xQueueHandle interruptQueue;


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
	}
}


uint8_t get_keypad_pins() {
	//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
	//  Reading from read_from_PCF8574_pins
	uint8_t rx_data[1];

	i2c_master_read_from_device(I2C_NUM_0, PCF8574_SLAVE_KPD_ADDRESS, rx_data, 5, TIMEOUT_MS/portTICK_RATE_MS);

	//ESP_LOGI(TAG, "get_keypad_pins");
	//ESP_LOG_BUFFER_HEX(TAG, rx_data, 1);

	return rx_data[0]; // there is only one element in this array. Return it.
}

void set_keypad_pins(uint8_t data)
{
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	err = i2c_master_write_to_device(I2C_NUM_0, PCF8574_SLAVE_KPD_ADDRESS, data_t, len, 1000);
	if (err!=0)	ESP_LOGI(TAG, "Error in sending data to PCF8574 keypad");
/*	else
		ESP_LOGI(TAG, "set_keypad_pins successful");
*/
}

void find_key(){

	uint8_t byteOfColumns; //  used for storing what is read from PCF8574 keypad
	uint8_t bitSetOnColumn = 0;
	int col = 0;
	// ESP_LOGI(TAG, "byteOfColumns = get_keypad_pins()");
	byteOfColumns = get_keypad_pins();
	// find the column
	bitSetOnColumn = (255 - byteOfColumns)&0xf0;
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
	// ESP_LOGI(TAG, "set_keypad_pins(0x0f) - set all row pins high now");
	set_keypad_pins(0x0f); // set all row pins high now

	// find the row
	uint8_t byteOfRows; //  used for storing what is read from PCF8574 keypad
	// ESP_LOGI(TAG, "byteOfRows = get_keypad_pins()");
	byteOfRows = get_keypad_pins();

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
	xQueueSend(keyQueue, &keyPressed, ( portTickType ) 0); // after finding the key, put it in the queue

	// ESP_LOGI(TAG, "set_keypad_pins(0xf0)");
	set_keypad_pins(0xf0); // write 11110000 and re-initialize the keypad as both the column and the row are found by now



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
 * the LED_PIN according to the level of the INPUT_PIN.
 * At this time the ESP-IDF terminal prints the message that the GPIO was pressed,
 * its state, the pin Number and count.
 * The above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
 * It also calls the showKeyPressed() function to display the key
 */


void Key_Ctrl_Task(void *params) // this function will be called when interrupt happens.
{
	int pinNumber = 0;
	int count = 0;


	while (true)
	{
		if (xQueueReceive(interruptQueue, &pinNumber, portMAX_DELAY))
		{
			int pinState = gpio_get_level(INPUT_PIN);
			printf("GPIO%d state is %d. isKeyBeingRead %d\n", pinNumber, pinState, isKeyBeingRead);


			if (pinState == 0) // only when pin goes down (Falling edge)
			{
				isKeyBeingRead = true; // block more items from being added to the queue until this key is found
				printf("isKeyBeingRead is blocked %d\n", isKeyBeingRead);
				ESP_LOGI(TAG, "Key Pressed on PCF8574 keypad ");
				find_key();
				isKeyBeingRead = false; // reset flag so that the next key press can be added to the queue
				printf("GPIO%d was pressed %d times. Pin state is %d. isKeyBeingRead is released %d\n", pinNumber, ++count, pinState, isKeyBeingRead);
			}
		}
		else
		{
			ESP_LOGI(TAG, "Key NOT Pressed on PCF8574 keypad ");
		}
	}
	//vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32

}


void init_keypad()
{
	set_keypad_pins(0xf0); // write 11110000

	// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
	/*
	 * This way when the push button will be pressed, the state of the INPUT_PIN will
	 * go HIGH and when it will be released then the state of the INPUT_PIN will go LOW.
	 *
	 */
	// We configure the INPUT_PIN as a GPIO and then set its direction as an input.
	gpio_pad_select_gpio(INPUT_PIN);
	gpio_set_direction(INPUT_PIN, GPIO_MODE_INPUT);



	/* We enable the pullup resistor and disable the pulldown resistor on the input pin.
	 * https://www.best-microcontroller-projects.com/pcf8574.html says enable pull-up
	 * https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/
	 * The interrupt open-drain output pin is active LOW.  It is normally pulled HIGH
	 * using a pull-up resistor and is driven low by the PCF8574 when any of the inputs
	 * change state. This signals the MCU to poll the part to see what is going on.
	 * If connecting this pin, enable the internal pull-up resistor on the MCU or add
	 * an external pull-up of 10K or so.
	 */

	gpio_pulldown_dis(INPUT_PIN);
	gpio_pullup_en(INPUT_PIN);

	// Then, we will set the interrupt trigger type on the INPUT_PIN using gpio_set_intr_type() function.
	// This function takes in two parameters.
	// The first parameter is the GPIO pin number which is ‘INPUT_PIN’ in our case.
	// The second parameter is the trigger type which is set as ‘GPIO_INTR_NEGEDGE’ (falling)
	// which means the interrupt will be triggered when the the state of the pin changes from HIGH to LOW.
	// Refer falling edge per https://www.best-microcontroller-projects.com/pcf8574.html

	gpio_set_intr_type(INPUT_PIN, GPIO_INTR_NEGEDGE);

	/* Create a queue that can hold 10 elements, each of size int.
	 * Then create a task using xTaskCreate with necessary arguments. Refer https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
	 */
	interruptQueue = xQueueCreate(10, sizeof(int));
	xTaskCreate(Key_Ctrl_Task, "Key_Ctrl_Task", 2048, NULL, 1, NULL);
	/* Install the interrupt service routine with the flags for interrupt allocation
	 * Add the ISR handler for the INPUT_PIN using gpio_isr_handler_add()
	 */
	gpio_install_isr_service(0);
	gpio_isr_handler_add(INPUT_PIN, gpio_interrupt_handler, (void *)INPUT_PIN);

	// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

	keyQueue = xQueueCreate(32, sizeof(char)); // this queue holds the keys pressed. Its size as 32, is the total number of characters on a 1602 LCD display

}
