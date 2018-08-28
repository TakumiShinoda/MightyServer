#ifndef Html_h
#define Html_h

#include <Arduino.h>
#include <vector>
#include "ChainArray.h"

class Html{
  private:
    struct HtmlObject{
      String html;
      void (*prev)(ChainArray);
    };

  public:
    Html(String html, void (*prev)(ChainArray));
    String getHtml();
    struct HtmlObject htmlObj;
};

#endif
