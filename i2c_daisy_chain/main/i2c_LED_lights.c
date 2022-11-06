/* i2c - LED lights example

   Simple I2C example that shows LED lights blinking on PCF8574
   by writing to it and reading from it over I2C.
*/

#include "i2c_LED_lights.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

#define PCF8574_ADDR 0x21 // A0 jumper to ground

#define TIMEOUT_MS		1000
#define DELAY_MS		1000


static const char *TAG = "i2c_LED_lights";
// this function should be modified to return data of type uint8_t
//uint8_t read_from_PCF8574_pins()
void read_from_PCF8574_pins()
{

//  Reference https://embeddedexplorer.com/esp32-i2c-tutorial/
//  Reading from read_from_PCF8574_pins
	int len = 1;
    uint8_t rx_data[len];

//    while (1) {
    	i2c_master_read_from_device(I2C_NUM_0, PCF8574_ADDR, rx_data, len, TIMEOUT_MS/portTICK_RATE_MS);
    	ESP_LOG_BUFFER_HEX(TAG, rx_data, len);
    	vTaskDelay(DELAY_MS/portTICK_RATE_MS);
    	ESP_LOGI(TAG, "Read from PCF8574 pins");
  //  }
    //return rx_data;
}

void write_to_PCF8574_pins(uint8_t data)
{
	esp_err_t err;
	int len = 1;

	uint8_t data_t[len];
	data_t[0] = data;
	err = i2c_master_write_to_device(I2C_NUM_0, PCF8574_ADDR, data_t, len, 1000);

	vTaskDelay(DELAY_MS/portTICK_RATE_MS);
	if (err!=0) ESP_LOGI(TAG, "Error in sending data to PCF8574");

	// printf should be changed to ESP_LOGI
	printf("write_to_PCF8574_pins successful \n");

}

