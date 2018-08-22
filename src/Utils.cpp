#include "Utils.h"

std::array<String, 2> Utils::analyzeGetRequest(String request){
  std::array<String, 2> result;
  String path = "";
  String params = "";

  if(request.indexOf("GET") != 0) return result;
  split(request, ' ', 1).indexOf("/") == 0 ? path = split(request, ' ', 1) : path = "";
  if(path.indexOf("?") >= 0){
    params = path.substring(path.indexOf("?") + 1);
  }

  result[0] = path;
  result[1] = params;

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