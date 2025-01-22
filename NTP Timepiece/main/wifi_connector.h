// Referenced from https://esp32tutorials.com/esp32-web-server-esp-idf/

#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"

#ifndef WIFI_CONNECTOR_H // Include guard to prevent multiple inclusion / conditional compilation
#define WIFI_CONNECTOR_H

#define MY_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define MY_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#define MY_ESP_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

void init_nvs_flash(void);

void connect_wifi(void);

#endif //HEADER
