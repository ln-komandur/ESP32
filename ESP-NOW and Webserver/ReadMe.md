# IOT learning project with two ESP32 microcontrollers.


## Master with Sensors
One ESP32 acts as a client sending temperature and humidity data from a sensor to a server, without itself (client) being on any wifi network. It sends to the MAC address of the server using the ESP-NOW protocol.



## Slave hosting a Webserver
Another ESP32 microcontroller acts as the server. It receives data from multiple clients - only one in this example, connects  to the home wifi intranet, runs its own Webserver to publish  the data through a basic website and lets devices in the home wifi network access the data.


## Web page output
Basic web page shows sensor data in the phone browser as below


## References
Examples refered are from https://RandomNerdTutorials.com and https://LastMinuteEngineers.com

1. https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
2. https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/
