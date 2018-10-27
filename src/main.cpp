#include <Arduino.h>
#include <vector>
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "local_property.h"
#include "Utils.h"
#include "ChainArray.h"
#include "ServerObject.h"
#include "Html.h"
#include "ESPIFFS.h"
#include "WifiConnection.h"

ServerObject ServerObject;
Utils Utils;
ESPIFFS espiffs;
std::vector<uint8_t> Ports = {80};

void checkHeap(void *arg){
  while(true){
    Serial.print("Heap: ");
    Serial.println(esp_get_free_heap_size());
    delay(5000);
  }
}

void test(ChainArray queries, String *respHtml){
  std::vector<String> keys = queries.keys();

  for(int i = 0; i < keys.size(); i++){
    Serial.print("Query \'"+ keys[i] +"\': ");
    Serial.println(queries.get(keys[i]));
  }

  Serial.println(*(respHtml));
}

void reflectionApiCallback(ChainArray params, String *respHtml){
  std::vector<String> paramsKeys = params.keys();
  std::vector<uint8_t> keyHostFinds = Utils.vector_find(paramsKeys, String("host"));
  std::vector<uint8_t> keyUriFinds = Utils.vector_find(paramsKeys, String("uri"));
  std::vector<uint8_t> keyPortFinds = Utils.vector_find(paramsKeys, String("port"));
  int httpCode = 0;
  HTTPClient http;
  uint8_t port = 0;
  String uri = "";
  String host = "";

  keyHostFinds.size() > 0 ? host = params.get(paramsKeys[keyHostFinds[0]]) : host = "www.google.com";
  keyUriFinds.size() > 0 ? uri = params.get(paramsKeys[keyUriFinds[0]]) : uri = "/";
  keyPortFinds.size() > 0 ? port = params.get(paramsKeys[keyPortFinds[0]]).toInt() : port = 80;

  http.setUserAgent("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/28.0.1500.52 Safari/537.36");
  http.begin(host, port, uri);
  httpCode = http.GET();

  Serial.println(httpCode);
  httpCode > 0 ? *(respHtml) = http.getString() : *(respHtml) = "Failed";
}

void empty(ChainArray params, String *respHtml){
}

void setup(){
  Serial.begin(115200);
  
  if(!espiffs.begin()){
    Serial.println("SPIFFS failed");
  }

  if(!checkNetwork()){
    Serial.println("Not found Network SSID around here.");
    return;
  }

  if(!connectAP()){
    Serial.println("Fail to connect.");
    return;
  }

  xTaskCreatePinnedToCore(checkHeap, "checkHeap", 16384, NULL, 1, NULL, 1);

  Html addApiPage(espiffs.readFile("/addApi.html"), test);
  Html reflectionApi(String(" "), reflectionApiCallback);

  ServerObject.addServer(80);
  ServerObject.setResponse(80, "/reflect", &reflectionApi);
  ServerObject.setResponse(80, "/admin/addapi", &addApiPage);
  ServerObject.openAllServers();
}

void loop(){
  ServerObject.requestHandle({80});
}
