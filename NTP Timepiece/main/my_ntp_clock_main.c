// Referenced from https://esp32tutorials.com/esp32-sntp-esp-idf-synchronize-time-ntp/ 
/* LwIP SNTP example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif_sntp.h"
#include <lwip/netdb.h>
#include <wifi_connector.h>
#include <i2c_LCD.h>
#include <i2c_mast_handler.h>

#define I2C_NUM I2C_NUM_0 // The I2C hardware module (or I2C port) in the ESP32 being used to facilitate I2C communication. Can use I2C_NUM_0 or I2C_NUM_1.

#define NTP_SYNC_TIMEOUT_MS CONFIG_NTP_SYNC_TIMEOUT_MS
#define NTP_SERVER CONFIG_SNTP_TIME_SERVER
#define TIME_ZONE CONFIG_TIME_ZONE

#define REFRESH_MS	1000 // In how many milliseconds should the clock time be refreshed
#define LCD_SLAVE_ADDR CONFIG_LCD_SLAVE_ADDR // This is the same as 0x4E>>1 // change this according to your device setup
 
static const char *TAG = "NTP Tutorial";

time_t now;
struct tm timeinfo;

//const i2c_master_dev_handle_t LCD_device_handle;

void LCD_Refresh_Every_Second(void *params)
{
	// This tasks autonomously displays the date on the top line and time on the 2nd line of the LCD. It does not depend on any queue, interrupts or triggers
	i2c_master_dev_handle_t LCD_dev_handle = *(i2c_master_dev_handle_t  *)params;
	
	ESP_LOGI(TAG, "Now in LCD_Refresh_Every_Second task");

	UBaseType_t uxHighWaterMark; // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark
    /* Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark 
    * Optionally inspect our own high water mark on entering the task.
    */
    // uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark

	char strftime_buf[64];
	//char year[4];
	char date[24];
	
	while (true)
	
	{
		time(&now);
		localtime_r(&now, &timeinfo);
		strftime(strftime_buf, sizeof(strftime_buf), "%r", &timeinfo);
		//strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
		//strftime(year, sizeof(year), "%Y", &timeinfo);
		strftime(date, sizeof(date), "%a, %b-%d-%Y", &timeinfo);

		//ESP_LOGI(TAG, "The current year is %s Year ", year);
		ESP_LOGI(TAG, "The current date is %s ", date);
		write_string_on_LCD(LCD_dev_handle, 0, 0, date); // display date on line 1 of the LCD though as string

		ESP_LOGI(TAG, "The current time in the %s Time Zone is: %s", TIME_ZONE, strftime_buf);
		write_string_on_LCD(LCD_dev_handle, 1, 0, strftime_buf); // display time on line 2 of the LCD though as string

		vTaskDelay( REFRESH_MS / portTICK_PERIOD_MS); // Delay for REFRESH_MS and count next. Since vTaskDelay takes only xTicksToDelay as argument, it has to be divided by portTICK_PERIOD_MS which is the number of milliseconds a scheduler TICK takes

		/* 
		Refer - https://www.freertos.org/Documentation/02-Kernel/04-API-references/03-Task-utilities/04-uxTaskGetStackHighWaterMark
		Calling the function will have used some stack space, we would therefore now expect uxTaskGetStackHighWaterMark() to return a 
       	value lower than when it was called on entering the task. 
       	
       	A task may query its own high water mark by passing NULL as the xTask parameter for the handle of the task being queried.
       	*/
        uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL ); // Refer - https://www.freertos.org/FreeRTOS_Support_Forum_Archive/July_2019/freertos_Understanding_uxTaskGetStackHighWaterMark_results_51c44e8598j.html

		ESP_LOGI(TAG, "Refresh_Every_Second_Task uxHighWaterMark = %u", uxHighWaterMark); // Refer - https://www.freertos.org/FreeRTOS_Support_Forum_Archive/July_2019/freertos_Understanding_uxTaskGetStackHighWaterMark_results_51c44e8598j.html
	}
	vTaskDelete(NULL); // added per https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32 at the end of the function to gracefully end the task:
}

void app_main(void)
{
    // Refer https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/system_time.html

    time(&now);
    localtime_r(&now, &timeinfo);

    // Set timezone per the configuration file
    setenv("TZ", TIME_ZONE, 1);
    tzset();


    // Is time set? If not, tm_year will be (1970 - 1900).
    if (timeinfo.tm_year < (2016 - 1900)) 
    {
        ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
	init_nvs_flash();
	connect_wifi();

	// Refer https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/system_time.html
	esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(NTP_SERVER);
	esp_netif_sntp_init(&config);
	if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(NTP_SYNC_TIMEOUT_MS)) != ESP_OK) 
	{
	    ESP_LOGI(TAG,"Failed to update system time within %ds timeout",NTP_SYNC_TIMEOUT_MS/1000);
	}

        // update 'now' variable with current time
        time(&now);
    }
    else 
    {
        ESP_LOGI(TAG, "Time is ALREADY set in the RTC.");
    }

    i2c_master_bus_handle_t bus_handle = get_i2c_bus_handle(I2C_NUM); // Get the i2c master bus handle at I2C_NUM
    i2c_master_dev_handle_t LCD_device_handle = get_i2c_device_handle(bus_handle, LCD_SLAVE_ADDR);
    lcd_init(LCD_device_handle);
    xTaskCreate(&LCD_Refresh_Every_Second, "LCD_Refresh_Every_Second", 2048, &LCD_device_handle, 1, NULL);
    ESP_LOGI(TAG, "Created LCD_Refresh_Every_Second");
		
}
