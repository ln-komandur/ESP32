# i2c Daisy Chaining PCF8574 with 8 LEDs, a 4x4 membrane matrix keypad and a LCD1602 



## Steps to import into Eclipse "Espressif IDF Project"

1.  Download the contents of the "i2c_daisy_chain" folder from github as a zip file. The parent level should have this "ReadMe.md" file, "Makefile" and a "CMakeLists.txt". The "main" folder with 11 files (.c , .h and others) should be a sub-folder under the parent fine
  
![Zip file to import](zip%20file%20to%20import.png)

2.  Create a new "Espressif IDF Project" in Eclipse

![New Imported Project](New%20Imported%20Project.png)

3.  Import the contents of zip file into this project

![Import Archive to Project](Import%20Archive%20to%20Project.png)

4.  Agree to overwrite CMakeLists.txt and all contents by clicking **Yes To All**

![Overwrite CMakeLists_txt](Overwrite%20CMakeLists_txt.png)

5.  Build the new project

![Build Project](Build%20Project.png)






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
