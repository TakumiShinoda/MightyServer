#include "ChainArray.h"

void ChainArray::add(std::vector<String> keys, std::vector<String> values){
  for(int i = 0; i < keys.size(); i++){
    struct chainArray element;

    element.key = keys[i];
    element.value = values[i];
    chainArrays.push_back(element);
  }
}

void ChainArray::clear(){
  chainArrays.clear();
}

String ChainArray::get(String key){
  for(int i = 0; i < chainArrays.size(); i++){
    if(chainArrays[i].key == key) return chainArrays[i].value;
  }

  return "";
}
