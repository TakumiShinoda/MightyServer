#include <Arduino.h>
#include <vector>
// #include "../lib/HTTPClient/src/HTTPClient.h"
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "local_property.h"
#include "Utils.h"
#include "ChainArray.h"
#include "ServerObject.h"
#include "Html.h"
#include "WifiConnection.h"

#include "pages/addApi.h"

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

void test(ChainArray queries, String respHtml){
  std::vector<String> keys = queries.keys();
  HTTPClient http;

  for(int i = 0; i < keys.size(); i++){
    Serial.print("Query \'"+ keys[i] +"\': ");
    Serial.println(queries.get(keys[i]));
  }

  http.setUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36");
  http.begin("https://www.google.co.jp");
  int httpCode = http.GET();

  Serial.println(httpCode);
  httpCode > 0 ? respHtml = http.getString() : respHtml = "Failed";

  Serial.println(respHtml);
}

void setup(){
  Serial.begin(115200);

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

void loop(){
  ServerObject.requestHandle({80});
}
