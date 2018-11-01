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