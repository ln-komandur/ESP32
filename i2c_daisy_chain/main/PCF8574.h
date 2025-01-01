#include <esp_err.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include <esp_timer.h>

uint8_t read_byte_from_pins(void); // read byte from pins

void write_byte_to_pins(uint8_t data); // write byte to pins

void init_LED(uint8_t startByte, long duration);

extern long blinkDuration; // in microseconds

extern esp_err_t err;
