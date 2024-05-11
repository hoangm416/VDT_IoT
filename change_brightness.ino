#include "WiFi.h"
#include <PubSubClient.h>
#include <stdlib.h>
#include <ArduinoJson.h>

#define LED_PIN 2
WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_server = "mqttvcloud.innoway.vn";
const char* mqttUser = "hoang";
const char* mqttPassword = "bDmAUtjW5cbbb8yMGqSbXgo273c0g4MI";

#define SSID "Hoang"
#define PASSWORD "12345678"


int casting(String a){
  int res = 0;
  for(int i = 1; i < a.length()-1; i++){
    res = res * 10 + (a[i] - '0');
  }
  return res;
}
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  

  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);

  
  if(strcmp(topic, "lightDim") == 0 ){
    Serial.println(messageTemp);
    int number = casting(messageTemp);
    Serial.println(number);
    analogWrite(LED_PIN, (number / 100.0) * 255);
  }
  
  Serial.println("Exit Call back");
}

void setup(){
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);  
  pinMode(LED_PIN, OUTPUT);

  
  WiFi.begin(SSID, PASSWORD);
  Serial.println("\nConnecting...");

    // while(WiFi.status() != WL_CONNECTED){
    //     Serial.print(".");
    //     delay(100);
    // }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);

}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32", mqttUser, mqttPassword)) {
      client.subscribe("lightDim");
      Serial.println("Connect Success");
    } else {
  
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(1000);
    }
  }
}

void loop(){
    if (!client.connected()) {
    reconnect();
  }
    client.loop();

}