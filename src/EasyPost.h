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
    StatusGen RespCode;
    StatusGen InternalStatus;

  public:
    String Status;
    EasyPost(Storage *_st);
    String addUser(String user, String pass);
    String updatePassword(String user, String _oldPass, String _newPass);
    int8_t statusCode();
    String addTable(String user, String pass, String tableName, std::vector<String> cols);
    String post(String user, String pass, String _tablePath, ChainArray data);
    // String get(String user, String pass);
};

#endif