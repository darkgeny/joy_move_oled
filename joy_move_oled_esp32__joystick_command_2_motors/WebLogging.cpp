// Author: Giacomo Catanzariti
#include "WebLogging.h"

extern uint8_t WLOG_CMD;

WebLogging::WebLogging(){}
WebLogging::WebLogging(uint8_t lp){
  set_pin(lp);
  digitalWrite(lp, HIGH);
  initialized = false;
}
void WebLogging::init(){
  initialized = true;
}
void WebLogging::set_pin(uint8_t p){
  ledpin = p;
}
uint8_t WebLogging::get_pin(){
  return ledpin;
}
void WebLogging::out(String s) {
  Serial.print(s);
}
void WebLogging::outln(String s) {
  Serial.println(s);
}

