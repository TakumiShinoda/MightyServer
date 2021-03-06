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

          Serial.print("From: ");
          Serial.println(client.remoteIP());

          Serial.print("Path: ");
          Serial.println(path);

          Serial.print("port: ");
          Serial.println(Servers[serverPos].port);

          Serial.print("size: ");
          Serial.println(Servers[serverPos].Responses.size());

          if(Servers[serverPos].Responses.size() == 0){
            sendGetResponseHeader(&client, "404", RESPTYPE_HTML);
            sendGetResponseBody(&client, notFoundResp);
          }else{
            for(int i = 0; i < Servers[serverPos].Responses.size(); i++){
              if(path == Servers[serverPos].Responses[i].url){
                String respHtml = Servers[serverPos].Responses[i].response;

                sendGetResponseHeader(&client, queries.get("ResponseStatus"), Servers[serverPos].Responses[i].respType);
                Servers[serverPos].Responses[i].prevCallback(queries, &respHtml, &client);
                sendGetResponseBody(&client, respHtml);
                break;
              }
              if(i == Servers[serverPos].Responses.size() - 1){
                sendGetResponseHeader(&client, "404", RESPTYPE_HTML);
                sendGetResponseBody(&client, notFoundResp);
                break;
              }
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

void ServerObject::sendGetResponseHeader(WiFiClient *client, String status, String respType){
  String newLine = "\r\n";
  String response = "";

  response += "HTTP/1.0 " + status + " OK" + newLine;
  response += "Content-Type: " + respType + newLine + newLine;

  for(int i = 0; i < response.length(); i++){
    client->print(response[i]);
  }
}

void ServerObject::sendGetResponseBody(WiFiClient *client, String html){
  for(int i = 0; i < html.length(); i++){
    client->print(html[i]);
  }
}

void ServerObject::setResponse(uint8_t port, String url, Html *response, String respType){
  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      if(Servers[i].findPath(url) >= 0){
        Servers[i].updateResponse(url, response, respType);
      }else{
        Servers[i].setResponse(url, response, respType);
      }
      break;
    }
  }
}

bool ServerObject::removeResponse(uint8_t port, String url){
  for(int i = 0; i < Servers.size(); i++){
    if(Servers[i].port == port){
      return Servers[i].removeResponse(utils->fixPath(url));
    }
  }
  return false;
}

void ServerObject::setNotFound(String resp){
  notFoundResp = resp;
}
