/*
 * i2c_daisy_chain_main.c
 * i2c - Daisy chain example
 *
 *  Created on: Jan 1, 2025
 *      Author: Ln-Komandur
 * LCD	1602 display, 4x4 Matrix Keypad and 8 LED lights daisy-chained over I2C using PCF8574 GPIO expander
 * Refer - https://embeddedexplorer.com/esp32-i2c-tutorial/
 */


#include <i2c_LCD.h>
#include <a_byte_of_LEDs.h>
#include <i2c-matrix-keypad.h>
#include <i2c_mast_handler.h>
#include <esp_log.h>
#include <PCF8574_IO_Expander.h>

#define I2C_NUM I2C_NUM_0 // The I2C hardware module (or I2C port) in the ESP32 being used to facilitate I2C communication. Can use I2C_NUM_0 or I2C_NUM_1.
#define PCF8574_SLAVE_ADDR CONFIG_PCF8574_SLAVE_ADDR // A0 jumper to ground
#define PCF8574_SLAVE_KPD_ADDRESS CONFIG_PCF8574_SLAVE_KPD_ADDRESS // No jumper set for keypad on PCF8574 expander
#define LCD_SLAVE_ADDR CONFIG_LCD_SLAVE_ADDR // This is the same as 0x4E>>1 // change this according to your device setup

#define ESP32_PIN_CONNECTED_TO_KEYPAD_INTERRUPT 15 // The interrupt pin 'INT' from PCF8574 is connected to GPIO15 of ESP32.
// ESP32_PIN_CONNECTED_TO_KEYPAD_INTERRUPT’ is used to read the digital input from pin number 15.

static const char *TAG = "i2c-daisy-chain-example";

long blinkDuration = 500000; // 500000 is in microseconds for 0.5 seconds

uint8_t byteToBlinkLEDs = 0x55; // Binary of 0x55 is 01010101. This is the byte that LEDs will blink for by shifting left.



void app_main(void)
{
	ESP_LOGI(TAG, "Begin porting new i2c_master.h driver");

	i2c_master_bus_handle_t bus_handle = get_i2c_bus_handle(I2C_NUM); // Get the i2c master bus handle at I2C_NUM
	
	ESP_LOGI(TAG, "Got i2c Bus Handle. Scanning for i2c Devices.");
	
	uint8_t *connectedAddresses = get_connected_i2c_device_addresses(bus_handle);

	for (uint8_t address = 0; (address < MAX_NO_OF_I2C_DEVICES) && (connectedAddresses[address] != 0x00); address++)
    {
        fflush(stdout);
		int len = 24;
		char buffer[len];
		sprintf(buffer, "i2c device at: 0x%02X\n", connectedAddresses[address]); // Load the address into the buffer and print
		printf("%s", buffer);
	}
	
	
	
	i2c_master_dev_handle_t LED_device_handle = get_i2c_device_handle(bus_handle, PCF8574_SLAVE_ADDR);
	init_LED_blink(LED_device_handle, byteToBlinkLEDs, blinkDuration);
	ESP_LOGI(TAG, "I2C initialized successfully via new i2c_driver.h");


	i2c_master_dev_handle_t LCD_device_handle = get_i2c_device_handle(bus_handle, LCD_SLAVE_ADDR);
	QueueHandle_t keyQueue; // A queue to be shared between the Key Pad and the LCD
	keyQueue = xQueueCreate(32, sizeof(char)); // this queue holds the keys pressed. Its size as 32, is the total number of characters on a 1602 LCD display

	struct LCD_Setup my_LCD_cfg = {LCD_device_handle, keyQueue};
	lcd_init(my_LCD_cfg);
	lcd_clear(my_LCD_cfg);

	//write_string_on_LCD(my_LCD_cfg,0,0, "Matrix keypad"); // A Sample text message

	i2c_master_dev_handle_t KPD_device_handle = get_i2c_device_handle(bus_handle, PCF8574_SLAVE_KPD_ADDRESS);
    struct passive_Matrix_keyPad_Setup my_kpd_cfg = {KPD_device_handle, ESP32_PIN_CONNECTED_TO_KEYPAD_INTERRUPT,keyQueue};
	init_keypad(my_kpd_cfg);
	
}
