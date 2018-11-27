#ifndef Utils_h
#define Utils_h

#include <Arduino.h>
#include <array>
#include <vector>
#include "ChainArray.h"

class StatusGen{
  private:
    ChainArray States;
    String RuleName;

  public:
    StatusGen(String name);
    void addStatus(int8_t code, String mes);
    void removeStatus(int8_t code);
    String getMes(int8_t code);
    String getArranged(int8_t code, String sep);
};

class Utils{
  public:
    ChainArray analyzeGetRequest(String request);
    String split(String target, char sep, uint8_t index);
    ChainArray analyzeQuery(String str);
    std::vector<uint8_t> vector_find(std::vector<String> target, String str);
    String ints2utf8(std::vector<uint8_t> data);
    String fixPath(String path);
};

#endif