# Install VPN

## NordVPN

### References

### Steps / Commands

#### If getting errors ####

-----------------------------------------
## Proton VPN

### References

1. [Installation steps](https://protonvpn.com/support/official-linux-vpn-ubuntu)
1. [Handling errors with pyopenssl cryptography](https://stackoverflow.com/questions/74981558/error-updating-python3-pip-attributeerror-module-lib-has-no-attribute-openss/75053968#75053968)


### Steps / Commands

`cd Downloads/`

`wget https://repo.protonvpn.com/debian/dists/stable/main/binary-all/protonvpn-stable-release_1.0.6_all.deb # From https://protonvpn.com/support/official-linux-vpn-ubuntu`

`sudo dpkg -i ./protonvpn-stable-release_1.0.6_all.deb && sudo apt update`

`sudo apt install proton-vpn-gnome-desktop # From https://protonvpn.com/support/official-linux-vpn-ubuntu`

`protonvpn-app`

#### If getting errors ####
 
`sudo pip show cryptography`

`sudo pip show pyOpenSSL`

`sudo pip install -U pyopenssl cryptography # From https://stackoverflow.com/questions/74981558/error-updating-python3-pip-attributeerror-module-lib-has-no-attribute-openss/75053968#75053968`

`sudo pip show pyOpenSSL`

`sudo pip show cryptography`
