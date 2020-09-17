# IOT learning project with two ESP32 microcontrollers.


## Master with Sensors

One ESP32 acts as a client sending temperature and humidity data from a sensor to a server, without itself (client) being on any wifi network. It sends to the MAC address of the server using the ESP-NOW protocol.

Code is as in https://github.com/ln-komandur/ESP32/blob/master/ESP-NOW%20and%20Webserver/ESP32NOW_PeerMaster_DHT11DataSender.ino

![Master with Sensors](https://github.com/ln-komandur/ESP32/blob/master/ESP-NOW%20and%20Webserver/IMG-20200910-WA0003.jpg)



## Slave hosting a Webserver

Another ESP32 microcontroller acts as the server. It receives data from multiple clients - only one in this example, connects  to the home wifi intranet, runs its own Webserver to publish  the data through a basic website and lets devices in the home wifi network access the data.

Code is as in https://github.com/ln-komandur/ESP32/blob/master/ESP-NOW%20and%20Webserver/ESP32NOW_PeerSlave_Receiver_WebServer.ino

![Slave hosting a Webserver](https://github.com/ln-komandur/ESP32/blob/master/ESP-NOW%20and%20Webserver/IMG-20200910-WA0004.jpg)


## Web page output

Basic web page shows sensor data in the phone browser as below.


![Browser Output](https://github.com/ln-komandur/ESP32/blob/master/ESP-NOW%20and%20Webserver/IMG-20200910-WA0005.jpg)

## References

Examples refered are from https://RandomNerdTutorials.com and https://LastMinuteEngineers.com

1. https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
2. https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/
