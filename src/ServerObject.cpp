#include "ServerObject.h"

void ServerObject::addServer(uint8_t port){
  struct Server server;
  WiFiServer s(port);

  server.port = port;
  server.server = s;

  Servers.push_back(server);
}

void ServerObject::openServer(uint8_t port){
  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      Servers[i].server.begin();
      break;
    }
  }
}

void ServerObject::requestHandle(uint8_t port){
  String line = "";
  String path = "";
  ChainArray request;
  ChainArray queries;
  std::vector<String> keys;
  WiFiClient client;

  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      client = Servers[i].server.available();
      break;
    }
  }

  if(client){
    if(client.connected()){
      delay(100);
      Serial.println("New Client");
      while(client.available()){
        line = client.readStringUntil('\r');
        path = "";

        if(line.indexOf("GET") >= 0){
          request = utils->analyzeGetRequest(line);
          path = request.get("path");
          queries = utils->analyzeQuery(request.get("params"));
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