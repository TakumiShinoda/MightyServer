#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "local_property.h"
#include "Utils.h"
#include "ChainArray.h"
#include "ServerObject.h"

#define TRY_CONNECT_AP 50

ServerObject ServerObject;
Utils Utils;

void checkHeap(void *arg){
  while(true){
    Serial.print("Heap: ");
    Serial.println(esp_get_free_heap_size());
    delay(5000);
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

bool checkNetwork(){
  bool result = false;

  for(int i = 0; i < WiFi.scanNetworks(); i++){
    String ssid = WiFi.SSID(i);

    if(ssid == SSID){
      result = true;
      break;
    }
  }

  return result;
}

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  xTaskCreatePinnedToCore(checkHeap, "checkHeap", 16384, NULL, 1, NULL, 1);

  if(!checkNetwork()){
    Serial.println("Not found Network SSID around here.");
    return;
  }

  if(!connectAP()){
    Serial.println("Fail to connect.");
    return;
  }

  ServerObject.addServer(80);
  ServerObject.addServer(88);

  ServerObject.setResponse(80, "/hogehoge", "hogehoge.com");
  ServerObject.setResponse(80, "/fugafuga", "fugafuga.com");

  ServerObject.openServer(80);
  ServerObject.openServer(88);
}

void loop() {
  ServerObject.requestHandle(80);
  ServerObject.requestHandle(88);
}
