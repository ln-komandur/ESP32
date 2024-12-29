# i2c Daisy Chaining PCF8574 with 8 LEDs, a 4x4 membrane matrix keypad and a LCD1602 



## Steps to import into Eclipse "Espressif IDF Project"

1.  Download the contents of the "i2c_daisy_chain" folder from github as a zip file. The parent level should have this "ReadMe.md" file, "Makefile" and a "CMakeLists.txt". The "main" folder with 11 files (.c , .h and others) should be a sub-folder under the parent fine
  
![Zip file to import](zip%20file%20to%20import.png)

2.  Create a new "Espressif IDF Project" in Eclipse and name it "i2c-daisy-chain"

![New Imported Project](New%20Imported%20Project.png)

3.  Click "File -> Import" and open the "Import Wizard"

![Import Wizard](Import%20Wizard.png)

4.  Import the contents of zip file into this project

![Import Archive to Project](Import%20Archive%20to%20Project.png)

5.  Agree to overwrite CMakeLists.txt and all contents by clicking **Yes To All**

![Overwrite CMakeLists_txt](Overwrite%20CMakeLists_txt.png)

6.  Build the new project

![Build Project](Build%20Project.png)

6.1 Build Successful

![Successful Build](Successful%20Build.png)


7. Flash the binaries to the ESP32 chip

7.1 Configure Serial port

If you clicked `Launch in Run mode` without configuring the serial port, you will get a `Serial port not found` error below

![Serial port not found](Serial%20port%20not%20found.png)

7.2 Select ESP Target

![Select ESP Target](ESP%20Target.png)

7.3 Select serial port

![Select serial port](Select%20serial%20port.png)

7.4 Setup the ESP Target

![Setup the ESP Target](Setup%20the%20ESP%20Target.png)

7.5 Flash the binaries to the ESP32 chip

![Flash Completed](Flash%20Completed.png)



## Fritzing circuit image
![Three i2c devices on PCF8574_bb](Three%20i2c%20devices%20on%20PCF8574_bb.png)


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

## References to upgrading versions
### ESP-IDF - from 4.4 to 5.0
[ESP-IDF » Migration Guides » Migration from 4.4 to 5.0 » Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/migration-guides/release-5.x/5.0/build-system.html)  

Add `#include "esp_timer.h"` wherever needed

Change `gpio_pad_select_gpio` to `esp_rom_gpio_pad_select_gpio`

#### CMakeLists.txt related
[C++ multiple definition linker issue with ESP-IDF](https://stackoverflow.com/questions/77633054/c-multiple-definition-linker-issue-with-esp-idf)

Edit the `CMakeLists.txt` inside the `main` folder to address the following build / compilation errors
```
multiple definition of `err';
multiple definition of `blinkDuration';
multiple definition of `keyQueue'; 
```

```
idf_component_register(
	SRCS 
		"i2c_daisy_chain_main.c" 
		"i2c_port.c" 
		"i2c_LCD.c" 
		"PCF8574.c" 
		"i2c-matrix-keypad.c"
	INCLUDE_DIRS "."
	REQUIRES esp_timer esp_driver_i2c 
)
```

### FreeRTOS - from FreeRTOS V7.x.x to V8.x.x
[New FreeRTOS Defined typedefs Names](https://freertos.org/Documentation/04-Roadmap-and-release-note/02-Release-notes/01-FreeRTOS-V8#new-freertos-defined-typedefs-names)

Add `#include "stdint.h"` to process `uint8_t` wherever needed

Change `xQueueHandle` to `QueueHandle_t`

Change `portTickType` to `TickType_t`

Change `portTICK_RATE_MS` to `portTICK_PERIOD_MS`





### Compiler error when adding code into 'components'.
[fatal error: driver/i2c.h: No such file or directory | #include <driver/i2c.h>](https://esp32.com/viewtopic.php?t=29660)
