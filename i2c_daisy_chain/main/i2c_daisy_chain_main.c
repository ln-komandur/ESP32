/* i2c - Daisy chain example

   Simple I2C example showing 8 LED lights on PCF8574 and LCD1602 on PCF8574 daisy-chained over I2C

*/
#include <i2c_port.h>
#include <i2c_LCD.h>
#include <PCF8574.h>
#include <i2c-matrix-keypad.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

#define DELAY_MS		1000

// the below are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/
#define INPUT_PIN 15
/* We have connected the interrupt from PCF8574 to GPIO15.
 * Therefore, we define a variable called ‘INPUT_PIN’ to hold the its number as 15.
 * This will be used later on in the code to read the digital input.
 */

// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

static const char *TAG = "i2c-daisy-chain-example";

//static bool isKeyPressed = true;

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
	// printf("In IRAM_ATTR ");
	int pinNumber = (int)args;
	xQueueSendFromISR(interruptQueue, &pinNumber, NULL);
}


void write_string_on_LCD(int lineNo, int colNo, char *str)
{
	lcd_put_cur(lineNo, colNo);
	lcd_send_string("				"); // Erases the existing content fully

	lcd_put_cur(lineNo, colNo);
	lcd_send_string(str);
}

void write_hex_on_LCD(int lineNo, int colNo, uint8_t hex)
{
	char buffer[16];
	sprintf(buffer, "0x%02X", hex); // display hexadecimal

	lcd_put_cur(lineNo, colNo);
	lcd_send_string(buffer);
}

void blink_LEDs_by_bit_shift()
{
	uint8_t aByte = 0x55; //  binary of 0x55 is 01010101
	uint8_t aFoundByte[1]; //  used for storing what is read from PCF8574
	/*
	 * 0x55 = 01010101, which,
	 * when shifted left once, it gives 0xaa = 10101010, which
	 * when shifted left once, it gives 0x54 = 01010100, which
	 * when shifted left once, it gives 0xa8 = 10101000, which
	 * when shifted left once, it gives 0x50 = 01010000, which
	 * when shifted left once, it gives 0xa0 = 10100000, which
	 * when shifted left once, it gives 0x40 = 01000000, which
	 * when shifted left once, it gives 0x80 = 10000000, which
	 * when shifted left once, it gives 0x00 = 00000000, when the byte is reset to 0x55
	 */
	while (true)
	{
		write_byte_to_pins(aByte);
		aByte = aByte << 1;
		if (aByte == 0x00) {
			aByte = 0x55;
		}
		aFoundByte[0] = read_byte_from_pins();
		ESP_LOGI(TAG, "Read byte from PCF8574 pins");
		ESP_LOG_BUFFER_HEX(TAG, aFoundByte , 1);

		write_hex_on_LCD(1, 0, aFoundByte[0]); // display it on line 2 of the LCD though as decimal
	}
}

void show_byte_with_LEDs(uint8_t aByte)
{
	write_byte_to_pins(aByte);
	ESP_LOGI(TAG, "show_byte_with_LEDs");
}

void showKeyPressed(void) // this method may be called based on interrupt. Yet to explore
{
	char pressedKey; //  used for storing what is read from PCF8574 keypad
	pressedKey = find_key();


	//	ESP_LOGI(TAG, "Key Pressed on PCF8574 keypad ");
	char buffer[16];
	sprintf(buffer, "Pressed %c", pressedKey); // display hexadecimal
	write_string_on_LCD(1, 0, buffer); // display it on line 2 of the LCD though as string

	vTaskDelay(DELAY_MS/portTICK_RATE_MS); // Show the key name on the LCD for a short time
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
			showKeyPressed();
			ESP_LOGI(TAG, "Key Pressed on PCF8574 keypad ");
			printf("GPIO %d was pressed %d times. The state is %d\n", pinNumber, count++, gpio_get_level(INPUT_PIN));
		}
		else
		{
			ESP_LOGI(TAG, "Key NOT Pressed on PCF8574 keypad ");
			write_string_on_LCD(1, 0, "None pressed "); // display it on line 2 of the LCD though as string
		}
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32

}

void app_main(void)
{

	ESP_ERROR_CHECK(i2c_master_init());
	ESP_LOGI(TAG, "I2C initialized successfully via i2c_port.c");
	scan_for_i2c_devices();
	lcd_init();
	lcd_clear();

	write_string_on_LCD(0,0, "Matrix keypad");

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

	/*
	 *
	 */
	interruptQueue = xQueueCreate(10, sizeof(int));
	xTaskCreate(Key_Ctrl_Task, "Key_Ctrl_Task", 2048, NULL, 1, NULL);
	/*
	 *
	 */
	gpio_install_isr_service(0);
	gpio_isr_handler_add(INPUT_PIN, gpio_interrupt_handler, (void *)INPUT_PIN);

	// the above are from https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/

	//blink_LEDs_by_bit_shift();
}
