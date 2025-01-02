# Try [Espressif-IDE - installation](ReadMe.md) first as the below might be obsolete and circuitous

# Installing Eclipse IDE for C++ Developers on Linux (Lubuntu 20.04.1 LTS)
## Goal:
To avoid using Arduino IDE altogether, and instead use Eclipse IDE for C++ Developers to program arduino microcontrollers, ESP32 microcontrollers 

## Pre-requisites - Key installations
Open the terminal with Ctrl + Alt + T

Update and upgrade the system with 

`sudo apt-get update && sudo apt-get upgrade`

Install JRE 14 using this command if not already installed

`sudo apt install openjdk-14-jre-headless`

The following were also used to satisfy pre-requisites

```
sudo apt-get install python-is-python3
sudo apt install python3-pip

```

## Summary of all pre-requisites

The below is the summary of all pre-requisites met by the steps that follow

```
$ java --version
openjdk 14.0.2 2020-07-14
OpenJDK Runtime Environment (build 14.0.2+12-Ubuntu-120.04)
OpenJDK 64-Bit Server VM (build 14.0.2+12-Ubuntu-120.04, mixed mode, sharing)

$ which git
/usr/bin/git

$ which python
/usr/bin/python

$ which python3
/usr/bin/python3

$ pip3 --version
pip 20.0.2 from /usr/lib/python3/dist-packages/pip (python 3.8)

$ lsb_release -a
No LSB modules are available.
Distributor ID: Ubuntu
Description:    Ubuntu 20.04.1 LTS
Release:        20.04
Codename:       focal

$ uname -r
5.4.0-59-generic

```




## Downloading and Installing Eclipse IDE for C++ Developers
Login as a User who has admin privilleges. This will be the user who will write ESP32 programs using Eclipse IDE 

[Download Eclipse for CPP for Linux x86_64](https://www.eclipse.org/downloads/packages/) . On systems with default settings, the `eclipse-cpp-2020-12-R-linux-gtk-x86_64.tar.gz` will be downloaded in the `$HOME/Downloads/` directory

Extract the tar installable to the home directory of the logged-in user with

`tar xvf ~/Downloads/eclipse-cpp-2020-12-R-linux-gtk-x86_64.tar.gz -C ~ # Extract the tar installable to the home directory`

Rename the eclipse directory as below to help easily distinguish from the workspace directory to be created later when starting eclipse

`mv ~/eclipse ~/eclipse-installation # Rename to help distinguish the installation directory and the workspace directory`

Create a symbolic link to the eclipse executable file with

`sudo ln -s ~/eclipse-installation/eclipse /usr/local/bin/ # Create a symbolic link to the eclipse executable file`

Also test if the executable file can be called from any directory with `eclipse` at the terminal and close it.

Now, create a .desktop file to be able to start eclipse from GUI menu's in the desktop with

Create a desktop icon for Eclipse-IDE

```
echo '[Desktop Entry]
Version = 2020‑12
Type = Application
Terminal = false
Name = Eclipse C/C++
Exec = /usr/local/bin/eclipse
Icon = /home/the_actual_user_name/eclipse-installation/icon.xpm
Categories = Education;
' | sudo tee /usr/share/applications/eclipse.desktop # Create a desktop icon for Eclipse-IDE
```

## Allow dialout

Add identified users to the `dialout` group. Only then will those users be allowed to write to the (USB) port to which the ESP32 (or any other microcontroller board like Arduino) will be connected for flashing

`sudo usermod -a -G dialout $USER # Adds the super-user who is also the current user to the dialout group`

`sudo usermod -a -G dialout <named-user> # Adds the named-user who is NOT the current user to the dialout group`
__Note:__ The above command will allow any named non super-user to also use the Espressif-IDE without any issues from their own login

`shutdown -r now # Reboot or reinitiate the session (logout and log back in)`

`id -Gn # Check that the logged in user is a member of the dialout group` 

### Start Eclipse

1. Start eclipse after login either from the desktop menu (preferrable) or the terminal window with `eclipse`
1. Select a workspace and then go with the prompts from there

## Installing support for ESP32 within Eclipse
1. First and foremost, create a separate workspace in Eclipse for ESP32 Projects.  
1. Obtain [ESP-IDF 4.0 or above](https://github.com/espressif/esp-idf/releases) using the [instructions](https://github.com/espressif/esp-idf/releases)
1. Install it by running `. ./install.sh` and then `. ./export.sh` . [Refer](https://medium.com/@prabhakarpanday/programming-esp32-using-esp-idf-for-tensorflow-lite-f173eec91c01) as needed
1. Follow [these instructions](https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md) to complete the installation from within Eclipse

## Semantic errors
1. When trying examples like "Hello World", it is likely that Eclipse compiles successfully but still gives semantic errors. In that case, in the Preferences of the selected project inside Eclipse, go to _"C/C++ General -> Code Analysis -> Launching"_. Make sure that both check boxes are unchecked. Close and reopen the project or restart eclipse and rebuild the project.

## Other ESP32 references
1. [Espressif highly recommend installing the ESP-IDF through your favorite IDE](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html#ide)
1. [Eclipse compiles successfully but still gives semantic errors](https://stackoverflow.com/questions/16786555/eclipse-compiles-successfully-but-still-gives-semantic-errors/16891713)

## Installing support for Arduino within Eclipse

1. First and foremost, __create a separate workspace in Eclipse for Arduino Projects__
1. Click _"Help -> Eclipse Marketplace"_
1. Find _"sloeber"_
1. Install _"The Arduino Eclipse Plugin named Sloeber (V4)"_ (with the icon in a rectangular blue box)) and follow the prompts
1. After installing the plugin, go to the menu _"Arduino"_ and click _"Preferences". Under "Arduino -> Platforms and Boards"_ drill down to _"arduino -> Arduino AVR Boards-> 1.8.3"_ (for Arduino Mego 2560) and apply and close

## Key points for ESP32 and Arduino support to co-exist in the same eclipse instance
1. Create exclusive workspaces in Eclipse for ESP32 Projects and Arduino Projects. This helps maintaining build configurations exclusive to each microcontroller, without stepping over onto the other's. 
1. Be mindful that these 2 microcontroller families are compiled (built) and flashed differently from within Eclipse, as also how the port names for their respective launch targets are managed.
