
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

 ## Clone esp-idf from git

 `cd # Go to the home directory`
 
 `git clone -b v5.2.3 --recursive https://github.com/espressif/esp-idf.git esp-idf-v5.2.3 # Clone esp-idf from git`

 ## Get into the esp-idf directory and execute scripts
 `cd esp-idf-v5.2.3/ # Get into the esp-idf directory`
 
 `. ./install.sh # Execute the install shell script`
 
 `. ./export.sh # Execute the export shell script`

`cd Espressif-IDE/`






## Allow dialout

`sudo usermod -a -G dialout $USER`
