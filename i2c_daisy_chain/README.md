# i2c Daisy Chaining LCD1602 and 8 LED bulbs to PCF8574



## Steps to import into Eclipse "Espressif IDF Project"

1.  Download the contents of the "i2c_daisy_chain" folder from github as a zip file. The parent level should have this "ReadMe.md" file, "Makefile" and a "CMakeLists.txt". The "main" folder with 11 files (.c , .h and others) should be a sub-folder under the parent fine
1.  Create a new "Espressif IDF Project" in Eclipse. Import the contents of zip file into this project and agree to overwrite all contents.
1.  Build the new project.


## Photos of circuit

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
