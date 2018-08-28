#include "ServerObject.h"

void ServerObject::addServer_proc(uint8_t port){
  struct Server server;
  WiFiServer s(port);

  server.port = port;
  server.server = s;

  Servers.push_back(server);
}

void ServerObject::addServer(uint8_t port){
  addServer_proc(port);
}

void ServerObject::addServer(std::vector<uint8_t> ports){
  for(int i = 0; i < ports.size(); i++){
    addServer_proc(ports[i]);
  }
}

void ServerObject::openServer_proc(uint8_t port){
  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      Servers[i].server.begin();
      break;
    }
  }
}

void ServerObject::openAllServers(){
  for(int i = 0; i < Servers.size(); i++){
    Servers[i].server.begin();
  }
}

void ServerObject::openServer(uint8_t port){
  openServer_proc(port);
}

void ServerObject::openServer(std::vector<uint8_t> ports){
  for(int i = 0; i < ports.size(); i++){
    openServer_proc(ports[i]);
  }
}

void ServerObject::requestHandle(uint8_t port){
  uint8_t serverPos = 0;
  String line = "";
  String path = "";
  ChainArray request;
  ChainArray queries;
  WiFiClient client;
  std::vector<String> keys;

  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      serverPos = i;
      client = Servers[serverPos].server.available();
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

          if(Servers[serverPos].Responses.size() == 0) {
            client.println("404");
          }else{
            for(int i = 0; i < Servers[serverPos].Responses.size(); i++){
              if(path == Servers[serverPos].Responses[i].url){
                client.println(Servers[serverPos].Responses[i].response);
                break;
              }
              if(i == Servers[serverPos].Responses.size() - 1) client.println("404");
            }
          }

          client.stop();

          request.clear();
          queries.clear();
        }
      }
    }
  }
}

void ServerObject::setResponse(uint8_t port, String url, String response){
  struct Response respObj;

  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      Servers[i].setResponse(url, response);
      break;
    }
  }
}
