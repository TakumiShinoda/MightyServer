#include <Arduino.h>
#include <WiFi.h>
#include "local_property.h"
#include "Utils.h"
#include "ChainArray.h"

#define TRY_CONNECT_AP 20

WiFiServer server(80);
Utils Utils;

void message_recive(){
  String line = "";
  String path = "";
  ChainArray queries;
  WiFiClient client = server.available();

  if(client){
    if(client.connected()){
      delay(100);
      Serial.println("New Client");
      while(client.available()){
        line = client.readStringUntil('\r');
        queries.clear();
        path = "";

        if(line.indexOf("GET") >= 0){
          path = Utils.analyzeGetRequest(line)[0];
          queries = Utils.analyzeQuery(Utils.analyzeGetRequest(line)[1]);

          Serial.print("Path: ");
          Serial.println(path);
          Serial.print("Query \'hoge\': ");
          Serial.println(queries.get("hoge"));
        }
      }
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
