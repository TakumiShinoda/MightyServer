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

// bool EasyPost::addTable(String user, String pass, String tableName){

// }

// bool EasyPost::post(String user, String pass, String data){

// }

// String EasyPost::get(String user, String pass){

// }