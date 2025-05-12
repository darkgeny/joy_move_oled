// Author: Giacomo Catanzariti
//at 9/5/2025
//Buzzer is not yet implemented on joy_move_oled
#include "Arduino.h"
#include "Buzzer.h"
//
Buzzer::Buzzer(){};
Buzzer::Buzzer(uint8_t p){
  set_pin(p);
}
void Buzzer::bip_ALERT(uint8_t duration){
return;
  tone(pin,987);
  delay(duration);
  noTone(pin);
}
void Buzzer::bip(uint8_t duration){
return;
  bip_ALERT(duration);
}
void Buzzer::bip_WARNING(uint8_t duration){
return;
  tone(pin,392);
  delay(duration);
  noTone(pin);
}
void Buzzer::bip_OK(uint8_t duration){
return;
  tone(pin,220);
  delay(duration);
  noTone(pin);
}
void Buzzer::play(uint8_t note, uint8_t duration){
return;
  tone(pin, note*2, duration);
  delay(duration); 
}
void Buzzer::alarm(uint8_t rep){
return;
  for(uint8_t r=1;r<rep;r++){
    for(uint8_t i=1;i<15;i++){
      int _n=200*r+1000/i,_d=8*r;
      if(_n<1000 && _d>10)
       play(_n,_d);
    }
    delay(30/r);
  }
}
uint8_t Buzzer::get_pin(){
  return pin;
}
void Buzzer::set_pin(uint8_t p){
  pin = p;
  pinMode(p, OUTPUT);
}

