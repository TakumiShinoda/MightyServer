#ifndef ServerObject_h
#define ServerObject_h

#include <Arduino.h>
#include <WiFi.h>
#include <vector>
#include "ChainArray.h"
#include "Utils.h"
#include "Html.h"

class Html;
class Utils;
class ServerObject{
  public:
    void addServer(uint8_t port);
    void addServer(std::vector<uint8_t> ports);
    void openServer(uint8_t port);
    void openServer(std::vector<uint8_t> ports);
    void openAllServers();
    void requestHandle(uint8_t port);
    void requestHandle(std::vector<uint8_t> ports);
    void setResponse(uint8_t port, String url, Html *response);

  private:
    void addServer_proc(uint8_t port);
    void openServer_proc(uint8_t port);
    void requestHandle_proc(uint8_t port);
    void sendGetResponse(WiFiClient *client, String html, String status);
    struct Response{
      String url;
      String response;
      void (*prevCallback)(ChainArray, String);
    };
    struct Server{
      uint8_t port;
      WiFiServer server;
      std::vector<struct Response> Responses;
      void setResponse(String url, Html *response){
        struct Response resObj;
        Html *test = response;

        resObj.url = url;
        resObj.response = response->getHtml();
        resObj.prevCallback = response->htmlObj.prev;
        Responses.push_back(resObj);
      };
    };
    std::vector<struct Server> Servers;
    Utils *utils = new Utils();
};

#endif
