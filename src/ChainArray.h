#ifndef ChainArray_h
#define ChainArray_h

#include <Arduino.h>
#include <vector>

class ChainArray{
  public:
    String get(String key);
    void add(std::vector<String> keys, std::vector<String> values);

  private:
    struct chainArray{
      String key = "";
      String value = "";
    };
    std::vector<struct chainArray> chainArrays;
};

#endif
