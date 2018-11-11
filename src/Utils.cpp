#include "Utils.h"

ChainArray Utils::analyzeQuery(String str){
  std::vector<String> blocks;
  std::vector<String> keys;
  std::vector<String> values;
  String block = "";
  ChainArray result;

  for(int i = 0; i < str.length(); i++){
    if(str[i] == '&'){
      blocks.push_back(block);
      block = "";
    }else{
      block += str[i];
      if(i == str.length() - 1) blocks.push_back(block);
    }
  }
  for(int i = 0; i < blocks.size(); i++){
    uint8_t equalPos = blocks[i].indexOf("=");

    if(equalPos >= 0){
      keys.push_back(blocks[i].substring(0, equalPos));
      values.push_back(blocks[i].substring(equalPos + 1));
    }
  }
  result.add(keys, values);

  return result;
}

ChainArray Utils::analyzeGetRequest(String request){
  ChainArray result;
  ChainArray queries;
  String path = "";
  String params = "";

  if(request.indexOf("GET") != 0) return result;
  split(request, ' ', 1).indexOf("/") == 0 ? path = split(request, ' ', 1) : path = "";
  if(path.indexOf("?") >= 0){
    params = path.substring(path.indexOf("?") + 1);
    path = path.substring(0, path.indexOf("?"));
  }

  result.add("path", path);
  result.add("params", params);

  return result;
}

String Utils::split(String target, char sep, uint8_t index){
  std::vector<String> results;
  uint8_t resultsLength = 0;
  String block = "";

  for(int i = 0; i < target.length(); i++){
    if(target[i] == sep){
      results.push_back(block);
      block = "";
    }else{
      block += target[i];
    }
  }

  resultsLength = results.size();
  if(index > resultsLength) index = 0;

  return results[index];
}

std::vector<uint8_t> Utils::vector_find(std::vector<String> target, String str){
  std::vector<uint8_t> result;

  for(int i = 0; i < target.size(); i++){
    if(target[i] == str) result.push_back(i);
  }

  return result;
}

String Utils::ints2utf8(std::vector<uint8_t> data){
  unsigned long long size = data.size();
  unsigned long long i = 0;
  String result = "";

  while(i < size){
    if(data[i] <= 127){
      result += char(data[i]);
      i += 1;
    }else if(data[i] <= 223){
      result += char(data[i]);
      result += char(data[i + 1]);
      i += 2;
    }else if(data[i] <= 239){
      result += char(data[i]);
      result += char(data[i + 1]);
      result += char(data[i + 2]);
      i += 3;
    }
  }

  return result;
}