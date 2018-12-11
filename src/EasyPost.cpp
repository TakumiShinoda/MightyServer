#include "EasyPost.h"

EasyPost::EasyPost(Storage *_st){
  st = _st;
  RespCode.addStatus(1, "Success");
  RespCode.addStatus(-1, "Already exist");
  RespCode.addStatus(-2, "Storage error");
  RespCode.addStatus(-3, "User is not Exist");
  RespCode.addStatus(-4, "Failed to log in");
  RespCode.addStatus(-5, "Table is not Exist");
  RespCode.addStatus(-6, "Params error");
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
    tableHeader = tableHeader.substring(0, tableHeader.length() - 1);
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
    String tablePath = userPath + "/" + _tablePath + ".ep";

    if(password == pass){
      if(st->exist(tablePath)){
        String result = "";
        String indexStr = st->readLine(tablePath);
        indexStr = indexStr.substring(0, indexStr.length() - 1);
        std::vector<String> indexs;
        std::vector<String> inputKeys = data.keys();
        uint8_t cnt = 0;

        while(true){
          String block = utils->split(indexStr, char(0x03), cnt);

          if(block == ""){
            break;
          }else{
            indexs.push_back(block);
          }
          cnt += 1;
        }

        for(int i = 0; i < indexs.size(); i++){
          if(i >= indexs.size() - 1){
            result += data.get(indexs[i]);
          }else{
            result += data.get(indexs[i]) + String(char(0x03));
          }
        }
        result += '\n';

        Serial.println(result);

        if(st->appendToFile(tablePath, &result, st->fileSize(tablePath))){
          return RespCode.getArranged(1, ": ");
        }else{
          return RespCode.getArranged(-6, ": ");
        }
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

String EasyPost::get(String user, String pass, String tablename, uint32_t start, uint8_t length){
  String userPath = "easypost/" + user;

  if(st->exist(userPath)){
    String password = st->readFile(userPath + "/password.ep");

    if(password == pass){
      String tablePath = userPath + "/" + tablename + ".ep";

      if(st->exist(tablePath)){
        String result = RespCode.getArranged(1, ": ") + char(0x02);

        result += st->readLine(tablePath, start + 1, length);
        std::string test(result.c_str());
        test = std::regex_replace(test, std::regex(String(char(0x03)).c_str()), ",");
        return String(test.c_str());
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