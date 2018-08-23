#ifndef Utils_h
#define Utils_h

#include <Arduino.h>
#include <array>
#include <vector>
#include "ChainArray.h"

class Utils{
  public:
    std::array<String, 2> analyzeGetRequest(String request);
    String split(String target, char sep, uint8_t index);
    ChainArray analyzeQuery(String str);
};

#endif
