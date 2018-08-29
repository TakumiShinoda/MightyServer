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
  requestHandle_proc(port);
}

void ServerObject::requestHandle(std::vector<uint8_t> ports){
  for(int i = 0; i < ports.size(); i++){
    requestHandle_proc(ports[i]);
  }
}

void ServerObject::requestHandle_proc(uint8_t port){
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

          Serial.print("port: ");
          Serial.println(Servers[serverPos].port);

          Serial.print("size: ");
          Serial.println(Servers[serverPos].Responses.size());

          if(Servers[serverPos].Responses.size() == 0){
            sendGetResponse(&client, "404", "404");
          }else{
            for(int i = 0; i < Servers[serverPos].Responses.size(); i++){
              if(path == Servers[serverPos].Responses[i].url){
                Servers[serverPos].Responses[i].prevCallback(queries);
                sendGetResponse(&client, Servers[serverPos].Responses[i].response, "200");
                break;
              }
              if(i == Servers[serverPos].Responses.size() - 1) sendGetResponse(&client, "404", "404");
            }
          }

          request.clear();
          queries.clear();
        }
      }
    }
  }
}

void ServerObject::sendGetResponse(WiFiClient *client, String html, String status){
  String contentLength = String(html.length());
  String statusResp = "HTTP/1.1 " + status + " OK";
  String contentLengthResp = "Content-Length: " + contentLength;
  String connectionResp = "Connection: close";
  String contentTypeResp = "Content-Type: text/html";
  String newLine = "\r\n";

  client->print(statusResp + newLine + contentLengthResp + newLine + connectionResp + newLine +contentTypeResp + newLine + newLine + html);
  client->stop();
}

void ServerObject::setResponse(uint8_t port, String url, Html *response){
  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      Servers[i].setResponse(url, response);
      break;
    }
  }
}
