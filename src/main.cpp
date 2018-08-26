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
  ChainArray request;
  ChainArray queries;
  std::vector<String> keys;
  WiFiClient client = server.available();

  if(client){
    if(client.connected()){
      delay(100);
      Serial.println("New Client");
      while(client.available()){
        line = client.readStringUntil('\r');
        path = "";

        if(line.indexOf("GET") >= 0){
          request = Utils.analyzeGetRequest(line);
          path = request.get("path");
          queries = Utils.analyzeQuery(request.get("params"));
          keys = queries.keys();

          Serial.print("Path: ");
          Serial.println(path);

          for(int i = 0; i < keys.size(); i++){
            Serial.print("Query \'"+ keys[i] +"\': ");
            Serial.println(queries.get(keys[i]));
          }

          client.println("hoge");
          client.stop();

          request.clear();
          queries.clear();
        }
      }
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
