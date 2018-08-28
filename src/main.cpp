#include <Arduino.h>
#include <vector>
#include "freertos/FreeRTOS.h"
#include "local_property.h"
#include "Utils.h"
#include "ChainArray.h"
#include "ServerObject.h"
#include "Html.h"

#include "pages/addApi.h"

#define TRY_CONNECT_AP 50

ServerObject ServerObject;
Utils Utils;
std::vector<uint8_t> Ports = {80};

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

void test(ChainArray queries){
  std::vector<String> keys = queries.keys();

  for(int i = 0; i < keys.size(); i++){
    Serial.print("Query \'"+ keys[i] +"\': ");
    Serial.println(queries.get(keys[i]));
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);

  if(!checkNetwork()){
    Serial.println("Not found Network SSID around here.");
    return;
  }

  if(!connectAP()){
    Serial.println("Fail to connect.");
    return;
  }

  xTaskCreatePinnedToCore(checkHeap, "checkHeap", 16384, NULL, 1, NULL, 1);

  Html addApiPage(addApi_html, test);

  ServerObject.addServer(80);
  ServerObject.setResponse(80, "/admin/addapi", &addApiPage);
  ServerObject.openAllServers();
}

void loop() {
  ServerObject.requestHandle({80});
}
