// This functions reconnects your ESP8266 to your MQTT broker
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");  
      // Subscribe or resubscribe to a topic
      // You can subscribe to more topics (to control more LEDs in this example)
      client.subscribe("test/test1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


/* This functions is executed when some device publishes a message to a topic that your ESP8266 is subscribed to
 Change the function below to add logic to your program, so when a device publishes a message to a topic that 
 your ESP8266 is subscribed you can actually do something
*/

void callback(String topic, byte* message, unsigned int length) {
  Serial.println("Message arrived on topic: ");
  Serial.println(topic);
  String command;
  
  for (int i = 0; i < length; i++) {
    command += (char)message[i];
  }  
  if(topic=="esp/test"){
    Serial.print("Topic == esp/test recieved ");
    Serial.println(command);
    if(command == "Sweep"){
      Serial.println("Sweep");
    }
    if (command == "Calibrate"){
      Serial.println("Calibrate");
    }
  }
}
