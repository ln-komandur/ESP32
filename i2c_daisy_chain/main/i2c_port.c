#include "i2c_port.h"
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"

static const char *TAG = "i2c_PORT";


//static esp_err_t i2c_master_init(void)
esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_NUM_0;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000, //  I2C fast mode 400KHz bus. Solves https://arduino.stackexchange.com/questions/19150/i2c-lcd-displaying-weird-characters
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, 0, 0, 0);
}


void scan_for_i2c_devices(void)
{
	// Original code is from https://gist.github.com/herzig/8d4c13d8b81a77ac86481c6c1306bb12

	 printf("i2c scan: \n");
	 for (uint8_t i = 1; i < 127; i++)
	 {
		int ret;
		i2c_cmd_handle_t cmd = i2c_cmd_link_create();
		i2c_master_start(cmd);
		i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, 1);
		i2c_master_stop(cmd);
		ret = i2c_master_cmd_begin(I2C_NUM_0, cmd, 100 / portTICK_RATE_MS);
		i2c_cmd_link_delete(cmd);

		if (ret == ESP_OK)
		{
			printf("Found device at: 0x%2x\n", i);
		}
	}

}
