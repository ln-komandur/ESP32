

void lcd_init (void);   // initialize lcd

void lcd_send_cmd (char cmd);  // send command to the lcd

void lcd_send_data (char data);  // send data to the lcd

void lcd_send_string (char *str);  // send string to the lcd

void lcd_put_cur(int row, int col);  // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear (void);

void write_string_on_LCD(int lineNo, int colNo, char *str);

void write_hex_on_LCD(int lineNo, int colNo, uint8_t hex);

void init_LCD(xQueueHandle keyQueue);


