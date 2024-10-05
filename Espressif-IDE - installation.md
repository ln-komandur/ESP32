
## Download [the latest Espressid-IDE for Linux](https://dl.espressif.com/dl/idf-eclipse-plugin/ide/Espressif-IDE-linux.gtk.x86_64/latest)

## Extract the tar gunzip file in the home directory`
`tar xvf ~/Downloads/Espressif-IDE-*.tar.gz -C ~ # Extract the tar gunzip file in the home directory`
## Create a symbolic link
`sudo ln -s ~/Espressif-IDE/espressif-ide /usr/local/bin/ # Create a symbolic link`

## Create a desktop entry for Espressif-IDE

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

## Install cmake
`sudo nala install cmake # Install cmake`

## Allow dialout

`sudo usermod -a -G dialout $USER`

## Configure Espressif-IDF

### 1-Open Espressif-IDF and select a separate directory for the ESP32's workspace

### 2-Check for updates

![Click Help -> Check for updates](1-Check%20for%20updates.png "Click Help -> Check for updates")

### 3-Restart to apply updates

![Restart to apply updates. And close the Welcome tab](2-Restart%20to%20apply%20updates.png "Restart to apply updates. And close the Welcome tab")

### 4-Open ESP-IDF Manager

**First close the Welcome tab**. Else it will completely hide the ESP-IDF Manager tab / tile

![Open Espressif -> ESP-IDF Manager](3-Open%20ESP-IDF%20Manager.png "Open Espressif -> ESP-IDF Manager")

### 5-Download ESP-IDF
![Download ESP-IDF](4-Download%20ESP-IDF.png "Click Add ESP-IDF, Select Version and Directory to download")

View the progress of the download should in the Progress tab

![ESP-IDF Download progressing](4a-Downloading.png "ESP-IDF Download progressing")


### Handle rule errors

If you get "[Unable to copy rules for OpenOCD to system directory, try running the eclipse with sudo command](https://github.com/espressif/idf-eclipse-plugin/issues/777#issuecomment-1574885512)", 

![Unable to copy rules for OpenOCD to system directory, try running the eclipse with sudo command](4b-OpenOCD%20error.png "Unable to copy rules for OpenOCD to system directory, try running the eclipse with sudo command")

Copy [this 60-openocd.rules file](https://github.com/espressif/openocd-esp32/blob/master/contrib/60-openocd.rules) to `/etc/udev/rules.d/` manually after changing its owner and group to `root:root`

### 6-Create New Espressif IDF project

![File -> New -> Espressif Project](5-New%20Espressif%20IDF-Project.png "File -> New -> Espressif Project")


![New IDF Project dialog](5a-New%20IDF%20Project%20dialog.png "New IDF Project dialog")






