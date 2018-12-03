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
      result = result + c;
    }
  }
  return result;
}

String Storage::readLine(String fn, uint16_t lineNum, uint8_t length){
  String result = "";
  File f = SD.open('/' + fn, FILE_READ);
  uint16_t lineCnt = 0;
  uint16_t savedCnt = 0;

  if(f.available()){
    String line = "";

    for(unsigned long i = 0; i < f.size(); i++){
      char c;
      f.seek(i);
      c = f.read();

      if(c == '\n'){
        if(lineCnt >= lineNum){
          result += line + "\n";
          savedCnt += 1;
          if(savedCnt == length){
            break;
          }
        }
        line = "";
        lineCnt += 1;
      }else{
        line += c;
      }
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

  f = SD.open('/' + fn, "w");

  if(f.available()){
    f.print(*(data));
    return true;
  }else{
    Serial.println("write failed");
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

bool Storage::checkActive(){
  if(!Available) return false;
  String fn = "accessCheck.txt";
  String text = "hogefugahogefuga";
  String result = "";

  if(!writeFile(fn, &text)) return false;
  result = readFile(fn);
  if(result.length() == text.length()) return true;
  else return false;
}

bool Storage::appendToFile(String fn, String *data, uint32_t pos){
  if(!Available) return false;
  String filename = "";
  String dirname = "";
  int64_t size = fileSize(fn);
  File f;
  File fr;

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

  fr = SD.open('/' + fn, "r");
  String last = "";
  if(pos > 0 && pos < size){
    fr.seek(pos);
  }else if(pos > 0){
    fr.seek(size - 1);
    last = String(char(fr.read()));
  }
  fr.close();

  f = SD.open('/' + fn, "w");
  if(pos > 0 && pos < size){
    f.seek(pos);
  }else if(pos > 0){
    f.seek(size - 1);
  }

  if(f.available()){
    f.print(last + *(data));
    return true;
  }else{
    Serial.println("write failed");
    return false;
  }
}

int64_t Storage::fileSize(String fn){
  if(!Available) return -1;
  File f;

  f = SD.open('/' + fn, "r");

  if(f.available()){
    return f.size();
  }else{
    return -1;
  }
}