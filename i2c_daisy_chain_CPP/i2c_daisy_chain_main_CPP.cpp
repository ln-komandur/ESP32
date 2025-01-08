#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include "i2c_mast_handler.hpp"
#include "PCF8574_device.hpp"
#include "PCF8574_IO_Expander.hpp"

#define I2C_NUM I2C_NUM_0 // The I2C hardware module (or I2C port) in the ESP32 being used to facilitate I2C communication. Can use I2C_NUM_0 or I2C_NUM_1.
#define PCF8574_SLAVE_ADDR 0x21 // A0 jumper to ground
#define PCF8574_SLAVE_KPD_ADDRESS 0x20 // No jumper set for keypad on PCF8574 expander
#define LCD_SLAVE_ADDR 0x27 // This is the same as 0x4E>>1 // change this according to your device setup

#define ESP32_PIN_CONNECTED_TO_KEYPAD_INTERRUPT 15 // The interrupt pin 'INT' from PCF8574 is connected to GPIO15 of ESP32.
// ESP32_PIN_CONNECTED_TO_KEYPAD_INTERRUPT’ is used to read the digital input from pin number 15.

static const char *TAG = "i2c-daisy-chain-example";
long blinkDuration = 500000; // 500000 is in microseconds for 0.5 seconds
uint8_t	aByte = 0x0f; // Just an initial byte to write
uint8_t byteToBlinkLEDs = 0x05; // Binary of 0x55 is 01010101. This is the byte that LEDs will blink for by shifting left.

extern "C" void app_main(void)
{
  ESP_LOGI(TAG, "Begin porting new i2c_master.h driver to CPP");
	i2c_mast_handler my_i2c_bus_A = i2c_mast_handler(I2C_NUM, GPIO_NUM_22,  GPIO_NUM_21, true);
	i2c_master_bus_handle_t bus_handle = my_i2c_bus_A.get_i2c_bus_handle();
	
	// The below is to scan for all i2c devices on the i2c master bus
	ESP_LOGI(TAG, "Scanning for i2c Devices connected to i2c bus.");

	uint8_t *connectedAddresses = my_i2c_bus_A.get_connected_i2c_device_addresses();

	for (uint8_t address = 0; (address < MAX_NO_OF_I2C_DEVICES) && (connectedAddresses[address] != 0x00); address++)
  {
    fflush(stdout);
    int len = 24;
    char buffer[len];
    sprintf(buffer, "i2c device at: 0x%02X\n", connectedAddresses[address]); // Load the address into the buffer and print
    printf("%s", buffer);
  }
	// 	The above is to scan for all i2c devices on the i2c master bus

	PCF8574_IO_Expander a_PCF8574_IO_Expander = PCF8574_IO_Expander(bus_handle, PCF8574_SLAVE_ADDR);
	a_PCF8574_IO_Expander.write_byte_to_pins(aByte);


  PCF8574_device myBlinkingLEDs = PCF8574_device(bus_handle, PCF8574_SLAVE_ADDR, blinkDuration, byteToBlinkLEDs);
  myBlinkingLEDs.start_blinking();
    
}
