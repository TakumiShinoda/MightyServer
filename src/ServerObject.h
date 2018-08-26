#ifndef ServerObject_h
#define ServerObject_h

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include "ChainArray.h"

class ServerObject{
  public:
    void addServer(uint8_t port);
  private:
    struct Server{
      uint8_t port;
      WiFiServer server;
    };
    std::vector<struct Server> Servers;
};

#endif