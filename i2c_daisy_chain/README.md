# i2c Daisy Chaining PCF8574 with 8 LEDs, a 4x4 membrane matrix keypad and an LCD1602 

## About this project

This project __Daisy Chains__ the following devices with __PCF8574 / PCF8574T I/O expanders__ for the __i2c bus__, and implements some interesting functionality showing in the sub-bulleted points below. It 

1. Scans for i2c devices
2. Reads bytes from PCF8574 pins
3. Writes bytes to PCF8574 pins
4. Uses TAGs and __Queues__
	1. Uses a TAG as "Matrix Keypad" to capture key strokes on the matric keypad and send them to the LCD
 	2. Uses a TAG as "PCF8574" to push the 8-bit string to blick the LED lights every 0.5 second
  	3. Handles __Tasks__
5. Assigns addresses for each i2c devices and reads from / writes to them (see each assigned address as below)
6. Blinks 8 LEDs of different colors
	1. Does a shift left of the bit pattern "0x55" whose binary is "01010101" continously with a delay of 1/2 a second and refreshes it upon "00000000"
 	2. At PCF8574_SLAVE_ADDR = __0x21__ _// A0 jumper to ground_
7. Reads from a 4x4 membrane matrix keypad
	1. At PCF8574_SLAVE_KPD_ADDRESS = __0x20__ _// No jumper set for keypad on PCF8574 expander_
 	2. __uses interrupts__ to begin recognizing the key that is pressed
  	3. Enables / disables __internal pullup / pulldown resistors__ available in the ESP32 for __certain GPIO pins__, and ties those pins to the interrupt pin of the PCF8574 in _the code_
 	4. uses some rudimentary logic to handle __debouncing key strokes__ by deciphering the key pressed as soon as and only after an interrupt has occured. After the key has been deciphered, the next interrupt is processed.
  	5. reads which key is pressed by masking the keypad's rows and columns 
8. Writes to an LCD1602
	1. At LCD Address = __0x27__ _// The same as 0x4E>>1_
	2. Writes a standard text on the first line
 	3. Writes the key strokes pressed on the second line
 
### References and courtesy credits
1. [ESP32-DevKitC V4 Getting Started Guide](https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/get-started-devkitc.html)
2. [PCF8574 GPIO Extender - Overview, Addressing, Finding address using i2c scanner, Using the Interrupt pin](https://how-to-videos.wonderhowto.com/forum/pcf8574-gpio-extender-with-arduino-and-nodemcu-0192441/)
3. [Reading from PCF8574 pins / i2c Tutorial](https://embeddedexplorer.com/esp32-i2c-tutorial/)
4. The LCD 1602A is based on the Hitachi HD44780 LCD controller. Refer
   - https://mil.ufl.edu/3744/docs/lcdmanual/commands.html#Sda 
   - [Interfacing LCD in 4-bit mode with 8051](https://exploreembedded.com/wiki/Interfacing_LCD_in_4-bit_mode_with_8051)
   - [RS=0 is to select command register](https://blog.embeddedexpert.io/?p=600)
   - In order to send data or command, we need to mask the two lines of the LCD which they are [EN line and RS line](https://blog.embeddedexpert.io/?p=655)
   - [Pin connection order in the notation LCD connection (to PCF8574 pin) is D7(P7), D6(P6), D5(P5), D4(P4), VEE(P3), EN(P2), RW (P1), RS (P0)](https://www.instructables.com/HD44780-LCD-to-I2C-adapter-board-for-the-Bus-Pirat/)
   - [The VEE pin regulates the display's contrast, via a changeable POT that can supply 0 to 5V](https://www.elprocus.com/lcd-16x2-pin-configuration-and-its-working/)
   - [Command set of the LCD 1602A which is based on the Hitachi HD44780 LCD controller](https://www.sparkfun.com/datasheets/LCD/HD44780.pdf) - The initialisation pattern is shown in page 46 of the datasheet
5. [i2c Clock speed](https://www.i2c-bus.org/speed/)
6. [i2c LCD Displaying Weird Characters - 400KHz SolveD](https://arduino.stackexchange.com/questions/19150/i2c-lcd-displaying-weird-characters)
7. [Scanning for i2c devices](https://gist.github.com/herzig/8d4c13d8b81a77ac86481c6c1306bb12)
8. [GPIO interrupts](https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/)
9. [Using FreeRTOS tasks](https://stackoverflow.com/questions/63634917/freertos-task-should-not-return-esp32)
10. Enabling the _internal_ __pullup resistor__ and disabling the  _internal_ __pulldown resistor__ on the input pin. The interrupt pin 'INT' from PCF8574 is connected to GPIO15 of ESP32. ‘INPUT_PIN’ is used to read the digital input from pin number 15 in _the code_.
   - [_The interrupt open-drain output pin is active LOW.  It is normally pulled HIGH using a pull-up resistor and is driven low by the PCF8574 when any of the inputs change state. This signals the MCU to poll the part to see what is going on. If connecting this pin, enable the internal pull-up resistor on the MCU or add an external pull-up of 10K or so._](https://www.mischianti.org/2019/01/02/pcf8574-i2c-digital-i-o-expander-fast-easy-usage/)
   - [_As we have already configured a pulldown resistor on this GPIO, there is __no need to add a physical resistor__ ourselves._](https://esp32tutorials.com/esp32-gpio-interrupts-esp-idf/)
   - [_Warning: There are no internal pullups you have to supply your own resistors._](https://www.best-microcontroller-projects.com/pcf8574.html)
11. [Triggering the interrupt on the _falling edge_ (HIGH to LOW)](https://www.best-microcontroller-projects.com/pcf8574.html)
12. [To use interrupts, it requires a separate pin to detect the __falling edge__](https://arduino.stackexchange.com/questions/85914/keypad-pin-as-an-interrupt-wiring-or-programming)
13. [How to connect an LCD to PCF8574](https://controllerstech.com/i2c-in-esp32-esp-idf-lcd-1602/)
14. FreeRTOS.org's most basic [__example__ of real-time scheduling is a real-time system that incorporates a keypad, LCD and control algorithm](https://freertos.org/Documentation/01-FreeRTOS-quick-start/01-Beginners-guide/01-RTOS-fundamentals#example)
15. [Using I2C with a 4×4 Matrix Keypad](https://www.makeriot2020.com/index.php/2020/10/05/using-i2c-with-a-4x4-matrix-keypad/)
16. Since the matrix keypad is a passive device (has not power supplied to it and only connects the masking bits applied to the columns and rows), a [button press is detected when it is pulled to low so configure the interrupt to trigger in falling edge](https://community.infineon.com/t5/PSOC-5-3-1/Matrix-keypad-component-interrupt-and-last-row-last-column-key-missing-problem/td-p/353688#.) (GPIO_INTR_NEGEDGE)
17. [Pull-up and pull-down resistors](https://www.electrosoftcloud.com/en/arduino-pull-up-pull-down-resistors/)  


## Fritzing circuit image
![Three i2c devices on PCF8574_bb](Three%20i2c%20devices%20on%20PCF8574_bb.png)

__Note:__ In the above circuit, on GPIO15, an additional external pullup resistor is added inspite of enabling the internal pullup resistor via code


## Photos of circuit without 4x4 membrane matrix keypad

### 1. ESP32 DevKitC-v4 connected to PCF8574 and HW-131 Power Supply Module 
![ESP32 DevKitC-v4 connected to PCF8574 and HW-131 Power Supply Module](20221106_1845-1.jpg)


### 2. LCD1602 daisy-chained via i2c to PCF8574
![LCD1602 daisy-chained via i2c to PCF8574](20221106_1845-2.jpg)


### 3. 8-LEDs on common GND connected to VCC via separate 10k resistors
![8-LEDs on common GND connected to VCC via separate 10k resistors](20221106_1845-3.jpg)


### 4. Closer look of resistors and VCC jumper wires from PCF8574 pinouts
![Closer look of resistors and VCC jumper wires from PCF8574 pinouts](20221106_1845-4.jpg)


### 5. Working LCD1602 and string of LEDs
![Working LCD1602 and string of LEDs](20221106_1845-5.jpg)

## More

![Importing to Espressif-IDE as Espressif IDF Project](Importing%20to%20Espressif-IDE%20as%20Espressif%20IDF%20Project.md)

![Upgrading FreeRTOS and esp-idf](Upgrading%20FreeRTOS%20and%20esp-idf.md)
