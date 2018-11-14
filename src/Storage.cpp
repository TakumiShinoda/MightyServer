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
  File f = SD.open('/' + fn, FILE_READ);

  if(f.available()){
    for(unsigned long i = 0; i < f.size(); i++){
      char c;

      f.seek(i);
      c = f.read();
      Serial.println(i);
      result = result + c;
    }
  }
  return result;
}

bool Storage::writeFile(String fn, String *data){
  if(!Available) return false;
  String filename = "";
  String dirname = "";
  File f;

  for(int i = 0; i < fn.length(); i++){
    char c = fn[fn.length() - 1 - i];

    if(c == '/'){
      dirname = fn.substring(0, fn.length() - i - 1);
      break;
    }else{
      filename = c + filename;
    }
  }

  if(!exist(dirname)){
    mkdir(dirname);
  }

  f = SD.open('/' + fn, FILE_WRITE);

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