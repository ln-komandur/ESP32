#include <stdio.h>

uint8_t read_byte_from_pins(void); // read byte from pins

void write_byte_to_pins(uint8_t data); // write byte to pins

void init_LED(uint8_t startByte, long duration);
