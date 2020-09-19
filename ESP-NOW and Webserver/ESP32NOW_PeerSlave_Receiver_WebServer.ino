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

#include <WebServer.h>

/*Put your SSID & Password*/
const char* ssid = "Username";  // Enter SSID here
const char* password = "Password";  //Enter Password here

// declare an object of WebServer library
WebServer server(80);
// End changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  int x;
  int y;
}struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};

// Modified to integrate the example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

float temperature;
float humidity;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].x = myData.x;
  boardsStruct[myData.id-1].y = myData.y;
  
  temperature = boardsStruct[myData.id-1].x;
  humidity = boardsStruct[myData.id-1].y;
  
  Serial.printf("Temperature value: %d \n", boardsStruct[myData.id-1].x);
  Serial.printf("Humidity value: %d \n", boardsStruct[myData.id-1].y);
  Serial.println();
}
// Modified to integrate the example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

void connectToWifiAndStartWebserver() {

  Serial.println("Connecting to ");
  Serial.println(ssid);

  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");

}

// End changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/


void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Access Point and Station (i.e. Both mode)
  WiFi.mode(WIFI_AP_STA); 
  
  // Modified to integrate the example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/
  connectToWifiAndStartWebserver();

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

// Begin changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/

void loop() {

  server.handleClient();
  
  // Commented off the below from RandomNerdTutorial Example
  // Access the variables for each board
  /*int board1X = boardsStruct[0].x;
  int board1Y = boardsStruct[0].y;
  int board2X = boardsStruct[1].x;
  int board2Y = boardsStruct[1].y;
  int board3X = boardsStruct[2].x;
  int board3Y = boardsStruct[2].y;
  delay(10000);  */

}


void handle_OnConnect() {
  server.send(200, "text/html", SendHTML(temperature,humidity)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float Temperaturestat,float Humiditystat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>ESP32 Weather Report</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr +="p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<div id=\"webpage\">\n";
  ptr +="<h1>ESP32 Weather Report</h1>\n";
  
  ptr +="<p>Temperature: ";
  ptr +=(int)Temperaturestat;
  ptr +="°C</p>";
  ptr +="<p>Humidity: ";
  ptr +=(int)Humiditystat;
  ptr +="%</p>";
  
  ptr +="</div>\n";
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}

// End changes from example on https://lastminuteengineers.com/esp32-dht11-dht22-web-server-tutorial/


