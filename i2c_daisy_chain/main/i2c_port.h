#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

//static esp_err_t i2c_master_init(void);

esp_err_t i2c_master_init(void);

void scan_for_i2c_devices(void);
