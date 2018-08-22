#include <Arduino.h>
#include <WiFi.h>
#include "local_property.h"
#include "Utils.h"

#define TRY_CONNECT_AP 20

WiFiServer server(80);
Utils Utils;

void message_recive(){
  WiFiClient client = server.available();
  if(client){
    Serial.println("New Client");

    if(client.connected()){
      while(client.available()){
        String line = client.readStringUntil('\r');
        if(line.indexOf("GET") >= 0){
          Serial.println(Utils.analyzeGetRequest(line)[0]);
          Serial.println(Utils.analyzeGetRequest(line)[1]);
        }
        delay(100);
      }
      client.println("hoge");
      client.stop();
    }
  }
}

bool connectAP(){
  uint8_t cnt = 0;

  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED && cnt < TRY_CONNECT_AP){
    delay(500);
    Serial.print(".");
    cnt += 1;
  }
  if(WiFi.status() != WL_CONNECTED) return false;

  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}

void setup() {
  Serial.begin(115200);

  if(!connectAP()){
    Serial.println("Fail to connect.");
  }

  server.begin();
}

void loop() {
  message_recive();
}
