/* Coded from examples on RandomNerdTutorials.com and LastMinuteEngineers.com
 * Author: L Komandur
 */

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

#include <esp_now.h>
#include <WiFi.h>

// Begin changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/
#include "DHT.h"

#define DHTTYPE DHT11   // DHT 11

// DHT Sensor
uint8_t DHTPin = 4; 
               
// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);                

float Temperature;
float Humidity;

// End changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0xF0, 0x08, 0xD1, 0xC8, 0x48, 0xC4};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id; // must be unique for each sender board
    int x;
    int y;
} struct_message;

//Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  
  // Begin changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/
  pinMode(DHTPin, INPUT);
  Serial.println("Assigned DHT Pin ");

  dht.begin();              
  Serial.println("Started DHT 11 sensor ");
  // End changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Set values to send
  myData.id = 1;
  // Begin changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

  Temperature = dht.readTemperature(); // Gets the value of temperature
  Humidity = dht.readHumidity(); // Gets the value of humidity 

  // Modified to integrate the example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

  myData.x = Temperature;
  myData.y = Humidity;

  // End changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
