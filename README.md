# Installing Eclipse C++ IDE on Linux (Lubuntu 20.04.1 LTS)

Open the terminal with Ctrl + Alt + T

Update and upgrade the system with 

`sudo apt-get update && sudo apt-get upgrade`

Install JRE 14 using this command

`sudo apt install openjdk-14-jre-headless`

Login as a User who has admin privilleges. This will be the user who will write ESP32 programs using Eclipse IDE 

Download Eclipse for CPP for Linux x86_64 from https://www.eclipse.org/downloads/packages/ . On systems with default settings, the eclipse-cpp-2020-12-R-linux-gtk-x86_64.tar.gz will be downloaded in the $HOME/Downloads/ directory

Extract the tar installable to the home directory of the logged in user with

`tar xvf $HOME/Downloads/eclipse-cpp-2020-12-R-linux-gtk-x86_64.tar.gz -C $HOME`

Rename the eclipse directory as below to help easily distinguish from the workspace directory to be created later when starting eclipse

`mv $HOME/eclipse $HOME/eclipse-installation`

Create a symbolic link to the eclipse executable file with

`sudo ln -s $HOME/eclipse-installation/eclipse /usr/local/bin/`

Also test if the executable file can be called from any directory with

`eclipse`

Now, create a .desktop file to be able to start eclipse from GUI menu's in the desktop with

`sudo nano /usr/share/applications/eclipse.desktop`

Add the following lines to this file and save it

```
[Desktop Entry]
Version = 2020‑12
Type = Application
Terminal = false
Name = Eclipse C/C++
Exec = /usr/local/bin/eclipse
Icon = /home/the_actual_user_name/eclipse-installation/icon.xpm
Categories = Education;
```

Now add the current user to the 'dialout' group with

`sudo usermod -a -G dialout $USER`

Only the above will let this user write to the USB port to which the ESP32 (or any other microcontroller board like Arduino) will be connected for flashing

Logout and Log back in. Or restart the system with `shutdown -r now`

# Start Eclipse

1. Start eclipse after login either from the desktop menu (preferrable) or the terminal window with `eclipse`
1. Select a workspace and then go with the prompts from there

# Installing support for Arduino within Eclipse

1. Click "Help -> Eclipse Marketplace"
1. Find "sloeber"
1. Install "The Arduino Eclipse Plugin named Sloeber (V4)" or "The Arduino Eclipse Plugin named Sloeber Product (V4)" and follow the prompts

# Installing support for ESP32 within Eclipse
1. Obtain ESP-IDF 4.0 or above from https://github.com/espressif/esp-idf/releases , 
1. Install it by running `. ./install.sh` and then `. ./export.sh` . The https://medium.com/@prabhakarpanday/programming-esp32-using-esp-idf-for-tensorflow-lite-f173eec91c01 site is a good reference
1. Follow the instructions in https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md to complete the installation from within Eclipse







