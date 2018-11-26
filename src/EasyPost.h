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
    String EmptyStr = "";
    Utils *utils = new Utils();

  public:
    String Status = "0: None";
    EasyPost(Storage *_st);
    String addUser(String user, String pass);
    String updatePassword(String user, String _oldPass, String _newPass);
    int8_t statusCode();
    // bool addTable(String user, String pass, String tableName);
    // bool post(String user, String pass, String data);
    // String get(String user, String pass);
};

#endif