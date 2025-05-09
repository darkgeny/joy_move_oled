// Author: Giacomo Catanzariti
#ifndef WEBLOGGING_H
#define WEBLOGGING_H
#include "Arduino.h"
//
class WebLogging{
  public:
    WebLogging();
    WebLogging(uint8_t);
    uint8_t get_pin();
    void out(String);
    void outln(String);
    void init();
  private:
    bool initialized;
    uint8_t ledpin;
    void set_pin(uint8_t);
};
#endif