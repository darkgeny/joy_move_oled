// Author: Giacomo Catanzariti
#ifndef BUTTON_H
#define BUTTON_H
#include "Arduino.h"
#include "Buzzer.h"
#include "WebLogging.h"
//
class Button{
  public:
    Button();
    Button(uint8_t, WebLogging *);
    Button(uint8_t, Buzzer *, uint8_t, WebLogging *);
    uint8_t get_pin();
    uint8_t check_button();
    void set_max_seconds_press(uint8_t);
    uint8_t get_max_seconds_press_reset();
  private:
    unsigned long get_count_milliseconds_press();
    void set_pin(uint8_t);
    void set_is_pressed();
    void set_is_not_pressed();
    void reset_count_press();
    void inc_count_press();
    void dec_count_press();
    uint8_t get_count_press();
    void set_max_seconds_press_reset(uint8_t);
    Buzzer *buzzer;
    uint8_t pin, count_press;
    bool is_pressed;
    uint8_t MAX_SECONDS_PRESS_RESET;
    void set_wlog( WebLogging * );
    WebLogging *wlog;
    unsigned long time_press_start, time_press_passed;
};
#endif
