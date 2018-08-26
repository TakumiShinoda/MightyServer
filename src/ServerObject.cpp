#include "ServerObject.h"

void ServerObject::addServer(uint8_t port){
  struct Server server;
  WiFiServer s(port);

  server.port = port;
  server.server = s;

  Servers.push_back(server);
}