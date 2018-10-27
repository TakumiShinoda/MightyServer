#ifndef SPIFFS_h
#define SPIFFS_h

#include <Arduino.h>
#include <FS.h>
#include <SPIFFS.h>

class ESPIFFS{
  public:
    bool begin();
    String readFile(String path);
    bool writeFile(String path, String value);
    void removeFile(String path);

  private:
    bool SPIFFSIni = false;
};

#endif