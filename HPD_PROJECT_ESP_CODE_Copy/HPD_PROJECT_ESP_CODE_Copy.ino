// ESP8266 with SD card reader
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
//SoftwareSerial mySerial(4,5,false,256); //rx,tx

String dataOut = "" ;

// Change the credentials below, so your ESP8266 connects to your router
const char* ssid = "NetworkTest123";
const char* password = "rongrong";
const char* mqttServer = "192.168.1.78";
const int mqttPort = 1883;
const char* mqttUser = "capstone";
const char* mqttPassword = "capstoneezpz";

// Initializes the espClient. You should change the espClient name if you have multiple ESPs running 
WiFiClient espClient;
PubSubClient client(mqttServer, 1883, espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
 
  client.publish("esp/test", "Hello from ESP8266");
  client.subscribe("esp/test");  
  
  Serial.println("Wifi + MQTT set up completed");
  delay(100);

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Only need to get data once
  if(Serial.available()){
    String data = Serial.readString();
    //Convert to char array 
    int data_len = data.length() + 1;
    char data_array[data_len];
    data.toCharArray(data_array,data_len);
    
    Serial.println(data);
    if(data.startsWith("NanoCalibration=")){
        client.publish("esp/data_sweep", data_array);
        Serial.println("NanoCalibration");
    }
    if (data.startsWith("NanoSweep=")){
        client.publish("esp/data_calibration", data_array);
        Serial.println("NanoSweep");
    }
  }
}
