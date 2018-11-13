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
    bool exist(String fn);  
    bool mkdir(String fn);
};

#endif