#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c.h>

esp_err_t i2c_master_init(i2c_port_t i2c_port_num);

void scan_for_i2c_devices(i2c_port_t i2c_port_num);
