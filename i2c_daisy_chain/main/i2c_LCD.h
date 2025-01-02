#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <unistd.h>

extern i2c_port_t i2c_port_num; 

void lcd_init (i2c_port_t i2c_port_num);   // initialize lcd at the I2C port number

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);

void write_string_on_LCD(int lineNo, int colNo, char *str);

void write_hex_on_LCD(int lineNo, int colNo, uint8_t hex);

void attach_queue_to_LCD(QueueHandle_t keyQueue);
