#include "./Storage.h"

Storage::Storage(uint8_t cs){
  CS = cs;
  if(SD.begin(CS)) Available = true;
}

bool Storage::begin(){
  if(Available) return true;
  if(SD.begin(CS)){
    Available = true;
    return true;
  }
  return false;
}

bool Storage::available(){
  return Available;
}

String Storage::readFile(String fn){ 
  String result = "";

  return result;
}

bool Storage::writeFile(String fn, String *data){
  if(!Available) return false;
  File f = SD.open('/' + fn, FILE_WRITE);

  if(f.available()){
    f.println(*(data));
    return true;
  }else{
    return false;
  }
}

bool Storage::exist(String fn){
  if(!Available) return false;
  return SD.exists('/' + fn);
}

bool Storage::mkdir(String fn){
  if(!Available) return false;
  return SD.mkdir('/' + fn);
}