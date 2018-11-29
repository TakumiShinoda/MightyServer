#ifndef Storage_h
#define Storage_h

#include <SD.h>

class Storage{
  private:
    uint8_t CS;
    bool Available = false;

  public:
    Storage(uint8_t cs);
    bool begin();
    bool available();
    String readFile(String fn);
    bool writeFile(String fn, String *data);
    bool appendToFile(String fn, String *data, uint32_t pos = 0);
    bool exist(String fn);  
    bool mkdir(String fn);
    bool checkActive();
    int64_t fileSize(String fn);
};

#endif