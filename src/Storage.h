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
};

#endif