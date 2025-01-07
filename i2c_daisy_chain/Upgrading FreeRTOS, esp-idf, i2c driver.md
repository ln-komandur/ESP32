## Upgrading FreeRTOS, esp-idf, i2c driver

## ESP-IDF - from 4.4 to 5.0
[ESP-IDF » Migration Guides » Migration from 4.4 to 5.0 » Build System](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/migration-guides/release-5.x/5.0/build-system.html)  - This will work for esp-idf v5.3.1 or v5.3.2 as targets

Add `#include "esp_timer.h"` wherever needed

Change `gpio_pad_select_gpio` to `esp_rom_gpio_pad_select_gpio`

### CMakeLists.txt related
[C++ multiple definition linker issue with ESP-IDF](https://stackoverflow.com/questions/77633054/c-multiple-definition-linker-issue-with-esp-idf)

Edit the `CMakeLists.txt` inside the `main` folder as below

```
# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS 
    	# list the source files of this component
       	i2c_daisy_chain_main.c 
	i2c_port.c 
	i2c_LCD.c 
	PCF8574.c
	i2c-matrix-keypad.c
    INCLUDE_DIRS   .    # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
```

### Header files `*.h`

[Declare the variables in the various / respective header files](https://stackoverflow.com/questions/57759461/multiple-definition-of-variables-esp32) as `extern` to address the following build / compilation errors
```
multiple definition of `err';
multiple definition of `blinkDuration';
multiple definition of `keyQueue'; 
```

`extern esp_err_t err;`

`extern long blinkDuration; // in microseconds`

`extern QueueHandle_t keyQueue; // used for storing what is read from PCF8574 keypad`


## FreeRTOS - from FreeRTOS V7.x.x to V8.x.x
[New FreeRTOS Defined typedefs Names](https://freertos.org/Documentation/04-Roadmap-and-release-note/02-Release-notes/01-FreeRTOS-V8#new-freertos-defined-typedefs-names)

Add `#include "stdint.h"` to process `uint8_t` wherever needed

Change `xQueueHandle` to `QueueHandle_t`

Change `portTickType` to `TickType_t`

Change `portTICK_RATE_MS` to `portTICK_PERIOD_MS`


## Compiler error when adding code into 'components'.
[fatal error: driver/i2c.h: No such file or directory | #include <driver/i2c.h>](https://esp32.com/viewtopic.php?t=29660) - This was not required to be implemented in the `CMakeLists.txt`. Use this link only as a reference


## Migrating from legacy i2c driver to the new i2c_master driver

**Note:** Migrating from legacy`#include <driver/i2c.h>` to the new `#include <driver/i2c_master.h>` would apply based on the version of the `esp-idf` being used. For e.g. The following were performed for the application code to take advantage of the newer driver in `esp-idf v5.3.2`.

1. Based on whether `I2C_NUM0` or `I2C_NUM1` are used on the ESP32 for the master bus, an `i2c bus handle` is obtained by providing more configuration settings such as `sda` and `scl` pin numnbers
2. Various devices are configured with their `HEX addresses` and other appropriate settings and their `device handle` is obtained. The `device handle` is used to add the device to the `i2c bus` on its `i2c bus handle`
3. Functions in various programs that use the devices are refactored to use either `bus handles` or `device handles` rather than directly use `I2c_NUM` or `HEX addresses of devices`
4. Values are passed to functions as `struct` (to mimic an object like approach)
5. Many settings such as `I2C_NUM0` or `I2C_NUM1`, `HEX addresses` of devices, `pin numbers` etc. are maintained in the main program (where the `app_main` function is present) from where buses and devices are configured and handled
