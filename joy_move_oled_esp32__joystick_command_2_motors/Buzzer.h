// Author: Giacomo Catanzariti
//at 9/5/2025
//Buzzer is not yet implemented on joy_move_oled
#ifndef BUZZER_H
#define BUZZER_H
#include "Arduino.h"
//
class Buzzer{
  public:
    Buzzer();
    Buzzer(uint8_t);
    void bip(uint8_t);
    void bip_ALERT(uint8_t);
    void bip_WARNING(uint8_t);
    void bip_OK(uint8_t);
    void play(uint8_t, uint8_t);
    void alarm(uint8_t);
    uint8_t get_pin();
  private:
    void set_pin(uint8_t);
    uint8_t pin;
};
#endif