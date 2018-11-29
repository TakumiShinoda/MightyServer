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
#include "EasyPost.h"

#include "WifiConnection.h"

#define SDCS 5

ServerObject ServerObject;
Utils Utils;
ESPIFFS espiffs;
std::vector<uint8_t> Ports = {80};
Storage st(SDCS);
Rsa rsa(5101, 4271);
EasyPost ep(&st);

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

void easypostAddUserCallback(ChainArray queries, String *response){
  std::vector<String> keys = queries.keys();
  std::vector<uint8_t> userKeysFound = Utils.vector_find(keys, "user");
  std::vector<uint8_t> passwordKeysFound = Utils.vector_find(keys, "password");
  String user = userKeysFound.size() > 0 ? queries.get(keys[userKeysFound[0]]) : "";
  String password = passwordKeysFound.size() > 0 ? queries.get(keys[passwordKeysFound[0]]) : "";

  if(user != "" && password != ""){
    if(!ep.statusCode() == 0){
      *(response) = ep.addUser(user, password);
    }else{
      *(response) = ep.Status;
    }
  }else{
    *(response) = "0: Params shortage";
  }
}

void easypostUpdatePassCallback(ChainArray queries, String *response){
  std::vector<String> keys = queries.keys();
  std::vector<uint8_t> userKeysFound = Utils.vector_find(keys, "user");
  std::vector<uint8_t> old_passwordKeysFound = Utils.vector_find(keys, "old_password");
  std::vector<uint8_t> new_passwordKeysFound = Utils.vector_find(keys, "new_password");
  String user = userKeysFound.size() > 0 ? queries.get(keys[userKeysFound[0]]) : "";
  String old_password = old_passwordKeysFound.size() > 0 ? queries.get(keys[old_passwordKeysFound[0]]) : "";
  String new_password = new_passwordKeysFound.size() > 0 ? queries.get(keys[new_passwordKeysFound[0]]) : "";

  if(user != "" && old_password != "" && new_password != ""){
    if(!ep.statusCode() == 0){
      *(response) = ep.updatePassword(user, old_password, new_password);
    }else{
      *(response) = ep.Status;
    }
  }else{
    *(response) = "0: Params shortage";
  }
}

void easypostAddTableCallback(ChainArray queries, String *response){
  std::vector<String> keys = queries.keys();
  std::vector<uint8_t> userKeysFound = Utils.vector_find(keys, "user");
  std::vector<uint8_t> passwordKeysFound = Utils.vector_find(keys, "password");
  std::vector<uint8_t> tableNameKeysFound = Utils.vector_find(keys, "tablename");
  std::vector<uint8_t> colsKeysFound = Utils.vector_find(keys, "cols");
  String user = userKeysFound.size() > 0 ? queries.get(keys[userKeysFound[0]]) : "";
  String password = passwordKeysFound.size() > 0 ? queries.get(keys[passwordKeysFound[0]]) : "";
  String tableName = tableNameKeysFound.size() > 0 ? queries.get(keys[tableNameKeysFound[0]]) : "";
  String cols = colsKeysFound.size() > 0 ? queries.get(keys[colsKeysFound[0]]) : "";
  std::vector<String> colsVector;
  uint8_t cnt = 0;

  if(user != "" && password != "" && cols != ""){
    while(true){
      String block = Utils.split(cols, ',', cnt);

      if(block != ""){
        colsVector.push_back(block);
      }else{
        break;
      }
      cnt += 1;
    }

    if(ep.statusCode() != 0){
      *(response) = ep.addTable(user, password, tableName, colsVector);
    }else{
      *(response) = ep.Status;
    }
  }else{
    *(response) = "0: Params shortage";
  }
}

void setup(){
  Serial.begin(115200);
  delay(1000);

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

  xTaskCreatePinnedToCore(checkHeap, "checkHeap", 4096, NULL, 1, NULL, 1);

  Html emptyApi("Empty", empty);
  Html reflectionApi(String(" "), reflectionApiCallback);
  Html addApiPage("/addApi.html", fromESPIFFS);
  Html serviceAddApi(String(""), addApiCallback);
  Html serviceRemoveApi(String(""), removeApiCallback);
  Html servicesEasyPostAddUser(String(""), easypostAddUserCallback);
  Html servicesEasyPostUpdatePass(String(""), easypostUpdatePassCallback);
  Html servicesEasyPostAddTable(String(""), easypostAddTableCallback);

  ServerObject.setNotFound(espiffs.readFile("/404.html"));
  ServerObject.addServer(80);
  ServerObject.setResponse(80, "/empty", &emptyApi);
  ServerObject.setResponse(80, "/admin/addapi", &addApiPage);
  ServerObject.setResponse(80, "/reflect", &reflectionApi);
  ServerObject.setResponse(80, "/services/addapi", &serviceAddApi);
  ServerObject.setResponse(80, "/services/removeapi", &serviceRemoveApi); 
  ServerObject.setResponse(80, "/services/easypost/adduser", &servicesEasyPostAddUser);
  ServerObject.setResponse(80, "/services/easypost/updatepassword", &servicesEasyPostUpdatePass);
  ServerObject.setResponse(80, "/services/easypost/addtable", &servicesEasyPostAddTable);
  ServerObject.openAllServers();
}

void loop(){
  ServerObject.requestHandle({80});
}