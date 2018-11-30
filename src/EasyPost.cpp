#include "EasyPost.h"

EasyPost::EasyPost(Storage *_st){
  st = _st;
  RespCode.addStatus(1, "Success");
  RespCode.addStatus(-1, "Already exist");
  RespCode.addStatus(-2, "Storage error");
  RespCode.addStatus(-3, "User is not Exist");
  RespCode.addStatus(-4, "Failed to log in");
  RespCode.addStatus(-5, "Table is not Exist");
  InternalStatus.addStatus(0, "Initialize error");
  InternalStatus.addStatus(1, "Available");
  InternalStatus.addStatus(-2, "Storage error");
  Status = InternalStatus.getArranged(0, ": ");

  if(!st->exist("easyPost")){
    if(st->mkdir("easyPost")){
      Status = InternalStatus.getArranged(1, ": ");
    }else{
      Status = InternalStatus.getArranged(1, ": ");
    }
  }else{
    Status = InternalStatus.getArranged(1, ": ");
  }
}

int8_t EasyPost::statusCode(){
  String result = utils->split(Status, ':', 0);

  return result.toInt();
}

String EasyPost::addUser(String user, String pass){
  if(!st->exist("easyPost/" + user)){ 
    if(st->writeFile("easyPost/" + user + "/password.ep", &pass)){
      return RespCode.getArranged(1, ": ");
    }else{
      return RespCode.getArranged(-2, ": ");
    }
  }else{
    return RespCode.getArranged(-1, ": ");
  }
}

String EasyPost::updatePassword(String user, String _oldPass, String _newPass){
  String userPath = "easyPost/" + user; 

  if(st->exist(userPath)){
    String oldPass = st->readFile(userPath + "/password.ep");

    if(oldPass == _oldPass){
      if(st->writeFile(userPath + "/password.ep", &_newPass)){
        return RespCode.getArranged(1, ": ");
      }else{
        return RespCode.getArranged(-2, ": ");
      }
    }else{
      return RespCode.getArranged(-4, ": ");
    }
  }else{
    return RespCode.getArranged(-3, ": ");
  }
}

String EasyPost::addTable(String user, String pass, String tableName, std::vector<String> cols){
  String userPath = "easyPost/" + user;

  if(st->exist(userPath)){
    String password = st->readFile(userPath + "/password.ep");
    String tablePath = userPath + "/" + tableName + ".ep";
    String sep = String(char(0x03));
    String tableHeader = "";

    for(int i = 0; i < cols.size(); i++){
      Serial.println(cols[i]);
      tableHeader += cols[i] + sep;
    }
    tableHeader += '\n';

    Serial.println(tableHeader);

    if(password == pass){
      if(!st->exist(tablePath)){
        if(st->writeFile(tablePath, &tableHeader)){
          return RespCode.getArranged(1, ": ");
        }else{
          return RespCode.getArranged(-2, ": ");
        }
      }else{
        return RespCode.getArranged(-1, ": ");
      }
    }else{
      return RespCode.getArranged(-4, ": ");
    }
  }else{
    return RespCode.getArranged(-3, ": "); 
  }
}

String EasyPost::post(String user, String pass, String _tablePath, ChainArray data){
  String userPath = "easyPost/" + user;

  if(st->exist(userPath)){
    String password = st->readFile(userPath + "/password.ep");
    String tablePath = userPath + "/" + _tablePath;

    if(password == pass){
      if(st->exist(tablePath)){
        // String index = 
        return RespCode.getArranged(1, ": ");
      }else{
        return RespCode.getArranged(-5, ": ");
      }
    }else{
      return RespCode.getArranged(-4, ": ");
    }
  }else{
    return RespCode.getArranged(-3, ": ");
  }
}

// String EasyPost::get(String user, String pass){

// }