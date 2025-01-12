# Espressif-IDE - installation


## Why these instructions

1.  The Espressif-IDE  comes pre-packaged with Eclipse and many essentials for Espressif. Its installation process is very polished (provided you know it) and so is its use. However [Espressif-IDE](https://github.com/espressif/idf-eclipse-plugin/blob/master/docs_readme/Espressif-IDE.md) page
    1.  provides no more than a [Download](https://github.com/espressif/idf-eclipse-plugin/blob/master/docs_readme/Espressif-IDE.md#downloads) link to the latest [Espressif-IDE-linux.gtk.x86_64](https://dl.espressif.com/dl/idf-eclipse-plugin/ide/Espressif-IDE-linux.gtk.x86_64/latest) release
    1.  only helps with *[Installing Espressif-IDE by Windows Offline Installer](https://github.com/espressif/idf-eclipse-plugin/blob/master/docs_readme/Espressif-IDE-Windows-Installer.md)* with screenshots
1.  On the other hand the [Eclipse CDT + ESP-IDF Eclipse Plugin (a.k.a. IEP plugin) installation approach](https://github.com/espressif/idf-eclipse-plugin/blob/master/README.md#Installation) is clunky. This page provides
    1.  anywhere between **minimal to no instructions** on installing **Espressif-IDE** as that is not their focus. All their screenshots are for the *Eclipse CDT + IEP Plugin update approach* and that too with some dated ones on *Help > Install New Software* for Mac.
    1.  A link they provide to the [Espressif-IDE documentation](https://docs.espressif.com/projects/espressif-ide/en/latest/) again doesn't address *Linux installations*
1. [Installing Eclipse IDE for C++ Developers on Linux](Installing%20Eclipse%20IDE%20for%20C++%20Developers%20on%20Linux.md) was first tried out in 2020 and is old

These instructions aim to bridge the gap for **Installing Espressif-IDE on Linux** . In this approach you only need to extract a `tar.gz` file, create a `symbolic link`, create a `desktop entry` and give `dialout permissions`. The only possible error you are likely to get is while *ESP-IDF Manager* tries to copy the `60-openocd.rules` file, and that can be easily resolved as explained below.

## Goals (of using Espressif-IDE):
1. To avoid using Arduino IDE altogether
1. To be able to directly leverage and use capabilities of FreeRTOS and esp-idf such as
    1. Multi tasking (multi threading as threads are the same as tasks in the FreeRTOS context)
    1. Queues
    1. Interrupt handling
1. To break free from the `setup()` and `loop()` methods in the Arduino IDE

## Pre-requisites


```
$ which java
/usr/bin/java

$ which git
/usr/bin/git

$ which python # This will show an empty result, which is good

$ which python3
/usr/bin/python3
```

The below are **python and pip requirements** for the steps that follow

`sudo nala install python3-pip # This may already be met`

`# sudo nala install python-is-python3 # This is not required` **Do not install python-is-python3**

`# sudo nala install cmake # This is not required` **Do not install cmake**



The below is the **summary of all pre-requisites met** by the steps that follow

```
$ java --version
openjdk 19.0.2 2023-01-17
OpenJDK Runtime Environment (build 19.0.2+7-Ubuntu-0ubuntu322.04)
OpenJDK 64-Bit Server VM (build 19.0.2+7-Ubuntu-0ubuntu322.04, mixed mode, sharing)

$ pip3 --version
pip 23.3.2 from /usr/local/lib/python3.10/dist-packages/pip (python 3.10)

$ lsb_release -a
No LSB modules are available.
Distributor ID:	Ubuntu
Description:	Ubuntu 22.04.5 LTS
Release:	22.04
Codename:	jammy

$ uname -r
6.8.0-45-generic
```




## [Download](https://docs.espressif.com/projects/espressif-ide/en/latest/downloads.html#downloads) the [latest Espressid-IDE for Linux](https://dl.espressif.com/dl/idf-eclipse-plugin/ide/Espressif-IDE-linux.gtk.x86_64/latest)

## Extract the tar gunzip file in the home directory`
`tar xvf ~/Downloads/Espressif-IDE-*.tar.gz -C ~ # Extract the tar gunzip file in the home directory`

**Note:** `Espressif-IDE-3.0.0-linux.gtk.x86_64.tar.gz` was used for this installation, and later upgraded to version 3.1.0 from within the IDE


## Create a symbolic link
`sudo ln -s ~/Espressif-IDE/espressif-ide /usr/local/bin/ # Create a symbolic link`

## Create a desktop entry for Espressif-IDE

Edit the path correctly in `Icon = /home/<user-who-installed-Espressif>`

```
echo '[Desktop Entry]
Version = 3.0.0
Type = Application
Terminal = false
Name = Espressif-IDE
Exec = /usr/local/bin/espressif-ide 
Icon = /home/<user-who-installed-Espressif>/Espressif-IDE/icon.xpm
Categories = Education;' | sudo tee /usr/share/applications/Espressif-IDE.desktop # Create a desktop entry for Espressif-IDE
```

## Allow dialout

Add identified users to the `dialout` group. Only then will those users be allowed to write to the (USB) port to which the ESP32 (or any other microcontroller board like Arduino) will be connected for flashing

`sudo usermod -a -G dialout $USER # Adds the super-user who is also the current user to the dialout group`

`sudo usermod -a -G dialout <named-user> # Adds the named-user who is NOT the current user to the dialout group`
__Note:__ The above command will allow any named non super-user to also use the Espressif-IDE without any issues from their own login, __if they are part of a group that belongs to a super-user__. This is because, though `~/Espressif-IDE/espressif-ide` reside in the super-user's `$HOME` directory after installation / extraction, its permissions are `-rwxr-xr-x`

`shutdown -r now # Reboot or reinitiate the session (logout and log back in)`

`id -Gn # Check that the logged in user is a member of the dialout group` 

## Configure Espressif-IDF

### 1-Open Espressif-IDF and select a separate directory for the ESP32's workspace

![Select a separate directory for ESP32 workspace](0-Select%20workspace.png "Select a separate directory for ESP32 workspace")

### 2-Check and install updates

![Click Help -> Check for updates](1-Check%20for%20updates.png "Click Help -> Check for updates")

Here is from where the IDE was upgraded to version 3.1.0

### 3-Restart to apply updates

![Restart to apply updates. And close the Welcome tab](2-Restart%20to%20apply%20updates.png "Restart to apply updates. And close the Welcome tab")

### 4-Open ESP-IDF Manager

**First close the Welcome tab**. Else it will completely hide the ESP-IDF Manager tab / tile

![Open Espressif -> ESP-IDF Manager](3-Open%20ESP-IDF%20Manager.png "Open Espressif -> ESP-IDF Manager")

### 5-Download ESP-IDF and install it
![Download ESP-IDF](4-Download%20ESP-IDF.png "Click Add ESP-IDF, Select Version and Directory to download")

**Note:** ESP-IDF comes with a `60-openocd.rules` file which the *ESP-IDF Manager* will copy to `/etc/udev/rules.d/` 

View the progress of the download should in the Progress tab

![ESP-IDF Download progressing](4a-Downloading.png "ESP-IDF Download progressing")

`esp-idf v5.3.1` was used for this installation and later upgraded to `esp-idf v5.3.2` from within the IDE


### Handle rule errors

#### If you get "Unable to copy rules for OpenOCD to system directory", 

![Unable to copy rules for OpenOCD to system directory, try running the `espressif-ide` as `sudo espressif-ide`](4b-OpenOCD%20error.png "Unable to copy rules for OpenOCD to system directory, try running the eclipse with sudo command")

1. [Try running the `espressif-ide` as `sudo espressif-ide`](https://github.com/espressif/idf-eclipse-plugin/issues/777#issuecomment-1574885512)", and then download ESP-IDF from the *ESP-IDF Manager*
2. Copy [this 60-openocd.rules file](https://github.com/espressif/openocd-esp32/blob/master/contrib/60-openocd.rules) to `/etc/udev/rules.d/` manually after changing its owner and group to `root:root` . A copy is also [available in this repo](./60-openocd.rules)

#### If the correct OpenOCD rules file already exists from a prior version of esp-idf, click "No" for this warning to not overwrite it

![Warning asking to overwrite existing OpenOCD rules file](4c-OpenOCD%20rules%20file%20exists.png "Warning asking to overwrite existing OpenOCD rules file")

And after clicking **No** the Tool will be activated with the existing OpenOCD rules file

![Tool Activated with the existing OpenOCD rules file](4d-Tool%20activated%20with%20existing%20OpenOCD%20rules.png "Tool Activated with the existing OpenOCD rules file")


### Go to the terminal and continue the ESP-IDF installation

`cd ~/esp-idf-v* # Go to the esp-idf installation for that version`

`chmod +x install.sh # Give execute permissions to install.sh`

`chmod +x export.sh  # Give execute permissions to export.sh`

`chmod +x /tools/idf.py # Give execute permissions to idf.py inside the tools directory`

`./install.sh # Execute ./install.sh at the esp-idf installation directory`

`./install.sh # Execute install.sh at the esp-idf installation directory` and get the following result

```
Successfully installed idf-component-manager-2.1.2
All done! You can now run:

  . ./export.sh
```

`. ./export.sh # Execute install.sh at the esp-idf installation directory` and get the following result

```
Done! You can now compile ESP-IDF projects.
Go to the project directory and run:

  idf.py build
```

### 6-Create New Espressif IDF project

![File -> New -> Espressif Project](5-New%20Espressif%20IDF-Project.png "File -> New -> Espressif Project")


![New IDF Project dialog](5a-New%20IDF%20Project%20dialog.png "New IDF Project dialog")

#### Building the project from the terminal

`cd ~/<your-ESP32-workspace-name>/<the-project-name>/ # Go to your workspace and to the project directory inside it`

`idf.py create-manifest # Create a manifest file (main/idf_component.yml) that defines the dependencies of the project.`

`idf.py build # Build the project <the-project-name>`

## How to uninstall?

This approach ***assumes the user's home to be the location*** of the Espressif_IDE or Eclipse IDE installation

**Note:** Close Espressif-IDE or Eclipse IDE prior to uninstallation. 

`ls -al ~ # List all folder and files including hidden ones under the user's home`

`rm -rf ~/esp-idf* # Remove the esp idf such as esp-idf-v5.3.1`

`rm -rf ~/.espressif/  # Remove the espressif hidden folder`

`rm -rf ~/ESP32-workspace/ # Remove the ESP32 workspace if needed`

### If Espressif-IDE needs to be uninstalled

`ls -l ~/Espressif-IDE/ # List the Espressif IDE installation folder in the user's $HOME`

`rm -rf ~/Espressif-IDE/ # Remove the Espressif IDE installation folder in the user's $HOME if Espressif IDE needs to be removed`

`sudo rm /usr/share/applications/Espressif-IDE.desktop # Remove the Espressif IDE desktop file`

`ls -l /usr/local/bin/espressif-ide # List the symbolic link to the Espressif IDE desktop file`

`sudo rm /usr/local/bin/espressif-ide # Remove the symbolic link to the Espressif IDE desktop file`

### If Eclipse-IDE needs to be uninstalled

`ls -l ~/eclipse-installation/ # List the Eclipse IDE installation folder in the user's $HOME`

`rm -rf ~/eclipse-installation/ # Remove the Eclipse IDE installation folder in the user's $HOME if Eclipse needs to be removed`

`sudo rm /usr/share/applications/eclipse.desktop # Remove the Eclipse IDE desktop file`

`ls -l /usr/local/bin/eclipse-ide # List the symbolic link to the Eclipse IDE desktop file`

`sudo rm /usr/local/bin/eclipse-ide # Remove the symbolic link to the Eclipse IDE desktop file`

## References

### Eclipse with GitHub
[Tutorial on how to use Eclipse with GitHub](https://github.com/maxkratz/How-to-Eclipse-with-Github)

### ESP32-DevKitC V4 - User Guide
[ESP32-DevKitC V4 - User Guide](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp32-devkitc/user_guide.html#get-started-esp32-devkitc-board-front)

[ESP32-DevKitC V4 - User Guide - downloadable PDF](https://docs.espressif.com/projects/esp-dev-kits/en/latest/esp32/esp-dev-kits-en-master-esp32.pdf)



