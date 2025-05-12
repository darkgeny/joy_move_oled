#include <stdio.h>
// Author: Giacomo Catanzariti
//at 9/5/2025
#include "Arduino.h"
#include "Menu.h"
//
Menu::Menu(){};
Menu::Menu(JoyMove *jm, LolinOled *lo, Button *bu, Bleutooth *bleu, WebLogging *wl) {
  joymove = jm;
  loled = lo;
  button = bu;
  bleut = bleu;
  wlog = wl;
  timep_in = 0;
  timep_out = 100;
  getready = true;
  old_textbuf=(char*)malloc(loled->get_cols()+1);
  linea=(char *)malloc(loled->get_cols()+1);
}
void Menu::main_page() {
  int cursory=-1,cursorx=-1;
  if (digitalRead(button->get_pin()) == HIGH) {
    if (!getready) {
      getready = joymove->ready();
      button->reset_count_press();
    }
    if (timep_in == 0)
      timep_in = millis();
    if (timep_out < millis() - timep_in) {
      timep_in = millis();
      loled->writeAtRow(joymove->get_txt_speed_A(), 0);
      loled->writeAtRow(joymove->get_txt_speed_B(), 1);
      loled->writeAtRow(joymove->get_txt_direction_X(), 2);
      loled->writeAtRow(joymove->get_txt_direction_Y(), 3);
      loled->writeAtRow(joymove->get_txt_direction_XY(), 4);
      loled->writeAtRow("---------------------", 5);
    }
  } else
    switch (button->check_button()) {  //check multiple gestures on pressing, continue switch after HIGH (release button)
      case 2:
        wlog->outln("2 seconds");
        joymove->outln("\n\n\n\n      BLUETOOTH\n\n\n");
        delay(1000);
        if( scan_for_devices() ){
          bluetooth_show_menu_devices();
          while( digitalRead(button->get_pin() ) == HIGH){  //press button to exit to main page
            cursory=move_up_down();
            if(cursory >= 0){
              Serial.print("cursory:");
              Serial.println(cursory);
            }
            cursorx=move_left_right();
            /*if(cursorx >= 0){
              Serial.print("cursorx:");
              Serial.println(cursorx);
            }*/
            if(cursorx == 1){
              confirm_selected_device();
              break;
            }
          }
        }
        loled->cursorStop();
        break;
      case 4:
        wlog->outln("4 seconds");
        joymove->outln("\n\n\n\n     CALIBRATION\n\n\n");
        delay(1000);
        if (digitalRead(button->get_pin()) == HIGH)
          joymove->calibration();
        break;
      case 6:
        wlog->outln("6 seconds");
        joymove->outln("\n\n\n\n    ANOTHER MENU\n\n\n");
        delay(1000);
        if (digitalRead(button->get_pin()) == HIGH)
          joymove->outln("bla bla bla");
        break;
      case 10:
        wlog->outln("10 seconds");
        wlog->outln("Reset!");
        ESP.restart();
        break;
      default:
        getready = false;
        break;
    }
}
bool Menu::scan_for_devices(){
  if( bleut->get_devnum() > 0 ){
    joymove->clear();
    return true;
  }
  joymove->outln("\n\n\n\n..WAIT DISCOVERY...\n\n\n\n");
  bleut->discovery();
  if( bleut->get_devnum() == 0 ){
    joymove->outln("\n\n\n\n  NO DEVICES FOUND\n\n\n\n");
    delay(2000);
    return false;
  }
  snprintf(linea, loled->get_cols()+1, "\n\n\n\FOUND %d DEVICES\n\n\n\n", bleut->get_devnum() );
  joymove->outln(linea);
  delay(2000);
  joymove->clear();
  return true;
}
void Menu::bluetooth_show_menu_devices(){
  for(int i=0;i<bleut->get_devnum();i++){
    loled->writeAtRow(bleut->get_device(i).name, i);
    if( bleut->get_device(i).is_connected == CONNECTED )
      loled->circle_on_text_row(i);
  }

  /*loled->get_textbuf(old_textbuf, 7);//save value line
  loled->writeAtRow(linea, 7);
  delay(2000);
  loled->writeAtRow(old_textbuf, 7);//restore value line

  loled->get_textbuf(old_textbuf, 7);
  loled->writeAtRow("*** SELECT DEVICE ***", 7);
  delay(2000);
  loled->writeAtRow(old_textbuf, 7);*/

  loled->cursorOn( bleut->get_devnum() );
}
void Menu::confirm_selected_device(){
  int choice=-1;
  old_textbuf[0]='\0';
  loled->get_textbuf(old_textbuf, 7);

  while( choice < 0 ){
    choice=move_left_right();
    loled->cursorBlink();
  }
  switch( choice ){
    case 1: loled->writeAtRow("***** CONFIRMED *****",7);break;//right
    case 0: loled->writeAtRow("****** ABORTED ******",7);return;//left= exit then exit to main
  }
  joymove->outln("\n\n\n\n..WAIT CONNECT...\n\n\n\n");

  if( bleut->connect(loled->get_row_selected() ) != CONNECTED ){
    joymove->outln("\n\n\n\nnot connected\n\n\n\n");
    return;
  }else{
    joymove->outln("\n\n\n\n\n\n    CONNECTED !!!    \n\n\n\n");
    delay(2000);
    bleut->set_is_connected(loled->get_row_selected());
  }
}
int Menu::move_up_down(){
  int cursory=-1;
  joymove->update();
  if ( ( joymove->get_joy().Y > ( joymove->get_joy().MY - JOY_APPROX_ROUND_CENTER_JOYSTICK ) ) && joymove->get_joy().dirY == JOY_UP  ) {
    cursory=loled->cursorUp();
    wait_recenteredY();
    Serial.println("JOY UP");
  }
  if ( ( joymove->get_joy().Y < ( joymove->get_joy().mY + JOY_APPROX_ROUND_CENTER_JOYSTICK ) ) && joymove->get_joy().dirY == JOY_DOWN  ) {
    cursory=loled->cursorDown();
    wait_recenteredY();
    Serial.println("JOY DOWN");
  }
  return cursory;
}
int Menu::move_left_right(){
  int cursorx=-1;
  joymove->update();
  if ( ( joymove->get_joy().X > ( joymove->get_joy().MX - JOY_APPROX_ROUND_CENTER_JOYSTICK ) ) && joymove->get_joy().dirX == JOY_LEFT  ) {
    cursorx=0;
    wait_recenteredX();
    Serial.println("JOY LEFT");
  }
  if ( ( joymove->get_joy().X < ( joymove->get_joy().mX + JOY_APPROX_ROUND_CENTER_JOYSTICK ) ) && joymove->get_joy().dirX == JOY_RIGHT  ) {
    cursorx=1;
    wait_recenteredX();
    Serial.println("JOY RIGHT");
  }
  return cursorx;
}
void Menu::wait_recenteredX(){
  while ( !joymove->is_centeredX() )
    joymove->update();
}
void Menu::wait_recenteredY(){
  while ( !joymove->is_centeredY() )
    joymove->update();
}