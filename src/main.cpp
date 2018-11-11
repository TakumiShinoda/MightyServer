#include <Arduino.h>
#include <SD.h>
#include <vector>
#include "WiFi.h"
#include <HTTPClient.h>
#include "freertos/FreeRTOS.h"
#include "local_property.h"
#include "Utils.h"
#include "ChainArray.h"
#include "ServerObject.h"
#include "Html.h"
#include "ESPIFFS.h"
#include "Storage.h"
#include "Rsa.h"

#include "WifiConnection.h"

#define SDCS 5

ServerObject ServerObject;
Utils Utils;
ESPIFFS espiffs;
std::vector<uint8_t> Ports = {80};
Storage st(SDCS);
Rsa rsa(101, 3001);

void checkHeap(void *arg){
  while(true){
    Serial.print("Heap: ");
    Serial.println(esp_get_free_heap_size());
    delay(5000);
  }
}

void empty(ChainArray params, String *respHtml){
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

void fromESPIFFS(ChainArray params, String *path){
  *(path) = espiffs.readFile(*(path));
}

void addApiCallback(ChainArray queries, String *response){
  std::vector<String> keys = queries.keys();
  std::vector<uint8_t> htmlKeysFound = Utils.vector_find(keys, "html");
  std::vector<uint8_t> pathKeysFound = Utils.vector_find(keys, "path");
  String html = htmlKeysFound.size() > 0 ? queries.get(keys[htmlKeysFound[0]]) : "Service not available.";
  String path = pathKeysFound.size() > 0 ? queries.get(keys[pathKeysFound[0]]) : "";
  Html targetApi(html, empty);

  if(path == ""){
    *(response) = "(0)Failed to upload.";
  }else{
    path = path.substring(0, 1) != "/" ? "/" + path : path;
    *(response) = "(1)Service started.";
    ServerObject.setResponse(80, path, &targetApi);
  }
}

void setup(){
  String test = "hahaha";

  Serial.begin(115200);
  delay(1000);
  xTaskCreatePinnedToCore(checkHeap, "checkHeap", 16384, NULL, 1, NULL, 1);

  while(!espiffs.begin()){
    Serial.println("SPIFFS Initializing");
  }

  while(!st.begin()){
    Serial.println("Storage Initializing");
  }

  if(st.writeFile("hoge.txt", &test)){
    Serial.println("Suc");
  }else{
    Serial.println("failed");
  }

  if(!checkNetwork()){
    Serial.println("Not found Network SSID around here.");
    return;
  }

  if(!connectAP()){
    Serial.println("Fail to connect.");
    Serial.println("Start AP mode.");
    WiFi.mode(WIFI_MODE_AP);
    WiFi.softAP("mighty", "mighty");
    delay(1000);
    WiFi.softAPConfig(ip, ip, subnet);
    WiFi.softAPIP();
  }

  Html reflectionApi(String(" "), reflectionApiCallback);
  Html addApiPage("/addApi.html", fromESPIFFS);
  Html serviceAddApi(String(""), addApiCallback);

  ServerObject.setNotFound(espiffs.readFile("/404.html"));
  ServerObject.addServer(80);
  ServerObject.setResponse(80, "/admin/addapi", &addApiPage);
  ServerObject.setResponse(80, "/reflect", &reflectionApi);
  ServerObject.setResponse(80, "/services/addapi", &serviceAddApi);
  ServerObject.openAllServers();
}

void loop(){
  ServerObject.requestHandle({80});
}