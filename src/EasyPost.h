#ifndef EasyPost_h
#define EasyPost_h

#include <Arduino.h>
#include "Storage.h"
#include "ChainArray.h"
#include "Utils.h"

class Utils;
class EasyPost{
  private:
    Storage *st;
    String Status = "0: None";
    String EmptyStr = "";
    Utils *utils = new Utils();

  public:
    EasyPost(Storage *_st);
    String addUser(String user, String pass);
    int8_t statusCode();
    // bool addTable(String user, String pass, String tableName);
    // bool post(String user, String pass, String data);
    // String get(String user, String pass);
};

#endif