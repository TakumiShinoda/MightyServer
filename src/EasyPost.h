#ifndef EasyPost_h
#define EasyPost_h

#include "Storage.h"

class EasyPost{
  private:
    Storage *st;

  public:
    EasyPost(Storage *_st);
    bool post(String user, String pass, String data);
    String get(String user, String pass);
};

#endif