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
Rsa rsa(5101, 4271);

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

  if(path == "" || html == ""){
    *(response) = "(0)Failed to upload.";
  }else{
    path = path.substring(0, 1) != "/" ? "/" + path : path;
    *(response) = "(1)Service started.";
    ServerObject.setResponse(80, path, &targetApi);
  }
}

void removeApiCallback(ChainArray queries, String *response){
  std::vector<String> keys = queries.keys();
  std::vector<uint8_t> pathKeysFound = Utils.vector_find(keys, "path");
  std::vector<uint8_t> portKeysFound = Utils.vector_find(keys, "port");
  String path = pathKeysFound.size() > 0 ? queries.get(keys[pathKeysFound[0]]) : "";
  String portStr = portKeysFound.size() > 0 ? queries.get(keys[portKeysFound[0]]) : "";
  uint16_t port = portStr.toInt();

  Serial.println("Port: " + portStr);
  Serial.println(port);
  Serial.println("hoge");
  if(path == "" || port == 0){
    *(response) = "(0)Failed to upload";
  }else{
    if(ServerObject.removeResponse(port, "/" + path)) *(response) = "(1)Removed /" + path;
    else *(response) = "(0)Failed to upload";
  }
}

void setup(){
  Serial.begin(115200);
  delay(1000);

  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  digitalWrite(18, LOW);
  digitalWrite(19, LOW);
  digitalWrite(21, LOW);
  digitalWrite(22, LOW);
  digitalWrite(23, LOW);

  while(!espiffs.begin()){
    Serial.println("SPIFFS Initializing");
  }

  while(!st.begin()){
    Serial.println("Storage Initializing");
    delay(500);
  }

  if(!st.checkActive()){
    Serial.println("SD is not activate");
    return;
  }

  Serial.println((char)rsa.decryption(rsa.encryption('a')));

  startAP();
  Serial.println("Start AP");

  if(!checkNetwork()){
    Serial.println("Not found Network SSID around here.");
  }else{
    if(!connectAP()){
      Serial.println("Fail to connect.");
    }
  }

  xTaskCreatePinnedToCore(checkHeap, "checkHeap", 16384, NULL, 1, NULL, 1);

  Html emptyApi("Empty", empty);
  Html reflectionApi(String(" "), reflectionApiCallback);
  Html addApiPage("/addApi.html", fromESPIFFS);
  Html serviceAddApi(String(""), addApiCallback);
  Html serviceRemoveApi(String(""), removeApiCallback);

  ServerObject.setNotFound(espiffs.readFile("/404.html"));
  ServerObject.addServer(80);
  ServerObject.setResponse(80, "/empty", &emptyApi);
  ServerObject.setResponse(80, "/admin/addapi", &addApiPage);
  ServerObject.setResponse(80, "/reflect", &reflectionApi);
  ServerObject.setResponse(80, "/services/addapi", &serviceAddApi);
  ServerObject.setResponse(80, "/services/removeapi", &serviceRemoveApi);
  ServerObject.openAllServers();
}

void loop(){
  ServerObject.requestHandle({80});
}