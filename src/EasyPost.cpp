#include "EasyPost.h"

EasyPost::EasyPost(Storage *_st){
  st = _st;

  if(!st->exist("easyPost")){
    if(st->mkdir("easyPost")){
      Status = "1: Available";
    }else{
      Status = "-2: Storage error";
    }
  }else{
    Status = "1: Available";
  }
}

int8_t EasyPost::statusCode(){
  String result = utils->split(Status, ':', 0);

  return result.toInt();
}

String EasyPost::addUser(String user, String pass){
  if(!st->exist("easyPost/" + user)){ 
    if(st->writeFile("easyPost/" + user + "/password.ep", &pass)){
      return "1: Success";
    }else{
      return "-2: Storage error";
    }
  }else{
    return "-1: Already exist";
  }
}

String EasyPost::updatePassword(String user, String _oldPass, String _newPass){
  String userPath = "easyPost/" + user; 

  if(st->exist(userPath)){
    String oldPass = st->readFile(userPath + "/password.ep");

    if(oldPass == _oldPass){
      if(st->writeFile(userPath + "/password.ep", &_newPass)){
        return "1: Success";
      }else{
        return "-2: Storage error";
      }
    }else{
      return "-4: Failed to log in";
    }
  }else{
    return "-3: User is not Exist";
  }
}

// bool EasyPost::addTable(String user, String pass, String tableName){

// }

// bool EasyPost::post(String user, String pass, String data){

// }

// String EasyPost::get(String user, String pass){

// }