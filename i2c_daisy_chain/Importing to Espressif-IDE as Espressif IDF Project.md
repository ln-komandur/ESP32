# Steps to import source code into Espressif-IDE as "Espressif IDF Project"

1.  Download the contents of the "i2c_daisy_chain" folder from github as a zip file. The parent level should have __this__ "ReadMe.md" file, a "CMakeLists.txt". The "main" folder with 11 files (.c , .h and others) should be a sub-folder under the parent fine.
	- __Note :__ You have to download a bigger zip file of the repo and then remove unnecessary files from it to create this zip file.

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

_6.1 Build Successful_

![Successful Build](Successful%20Build.png)

7.  Flash the binaries to the ESP32 chip

_7.1 Configure Serial port_

If you clicked `Launch in Run mode` without configuring the serial port, you will get a `Serial port not found` error below

![Serial port not found](Serial%20port%20not%20found.png)

_7.2 Select ESP Target_

![Select ESP Target](ESP%20Target.png)

_7.3 Select serial port_

![Select serial port](Select%20serial%20port.png)

_7.4 Setup the ESP Target_

![Setup the ESP Target](Setup%20the%20ESP%20Target.png)

_7.5 Flash the binaries to the ESP32 chip_

![Flash Completed](Flash%20Completed.png)
