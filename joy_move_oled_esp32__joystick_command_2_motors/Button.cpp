// Author: Giacomo Catanzariti
#include "Arduino.h"
#include "Button.h"
//
Button::Button(){}
Button::Button(uint8_t but, WebLogging *wl){
  set_pin(but);
  set_wlog( wl );
  reset_count_press();
}
Button::Button(uint8_t but, Buzzer *b, uint8_t max_seconds_press_reset, WebLogging *wl){
  set_pin(but);
  set_is_not_pressed();
  set_max_seconds_press_reset(max_seconds_press_reset);
  buzzer = b;
  set_wlog( wl );
}
uint8_t Button::get_pin(){
  return pin;
}
uint8_t Button::check_button(){ // checking the button press, wait until user lifts his finger from it 
  if(digitalRead(pin) == HIGH)
    if( time_press_start > 0 )
      reset_count_press();
  if(digitalRead(pin) == LOW){
    if( time_press_start == 0 )
      time_press_start = millis();
    if( 1000 < (millis() - time_press_start ) ){
      time_press_start = millis();
      inc_count_press();
      wlog->out("pressed ");
      wlog->out( (String)get_count_press() );
    }
    return get_count_press();
  }
  return 0;
}
void Button::set_pin(uint8_t p){
  pin = p;
  pinMode(p, INPUT);
}
void Button::set_is_pressed(){
  is_pressed = true;
}
void Button::set_is_not_pressed(){
  is_pressed = false;
}
void Button::reset_count_press(){
  count_press = 0;
  time_press_start = 0;
  wlog->out("reset");
}
void Button::inc_count_press(){
  count_press++;
}
void Button::dec_count_press(){
  count_press--;
}
uint8_t Button::get_count_press(){
  return count_press;
}
unsigned long Button::get_count_milliseconds_press(){
  return (count_press+1) * 1000;
}
void Button::set_max_seconds_press(uint8_t m){
  MAX_SECONDS_PRESS_RESET = m;
}
void Button::set_max_seconds_press_reset(uint8_t m){
  MAX_SECONDS_PRESS_RESET = m;
}
uint8_t Button::get_max_seconds_press_reset(){
  reset_count_press();
  return MAX_SECONDS_PRESS_RESET;
}
void Button::set_wlog( WebLogging *wl ){
  wlog = wl;
}
