/***********************************************************************
 Name: joy_move_oled
 Scope: convert joystick movements to motor speed driver and show to oled
 Copyright (C) 2025 Giacomo Catanzariti

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    1. You may copy and distribute verbatim copies of the Program's
  source code as you receive it, in any medium, provided that you
  conspicuously and appropriately publish on each copy an appropriate
  copyright notice and disclaimer of warranty; keep intact all the
  notices that refer to this License and to the absence of any warranty;
  and give any other recipients of the Program a copy of this License
  along with the Program.
************************************************************************/
#include "Arduino.h"
#include "JoyMove.h"
#include "Drive.h"

JoyMove::JoyMove(int px, int py, Drive *dr, LolinOled *ol){
  joy.pinX = px;
  joy.pinY = py;
  thedrive = dr;
  oled = ol;
  is_started = false;
  txtA = (char *)malloc(21);
  txtB = (char *)malloc(21);
  bufint = (char *)malloc(10);
}
void JoyMove::start(){
  begin();
  is_started = true;
}
void JoyMove::begin(){
  outln("Set default calibration.");
  joy.cX = PRESET_CENTER_X;
  joy.cY = PRESET_CENTER_Y;
  joy.mX = joy.mY = PRESET_MIN;
  joy.MX = joy.MY = PRESET_MAX;
  ready();
}
void JoyMove::begin(int px, int py){
  joy.cX = px;
  joy.cY = py;
  ready();
}
void JoyMove::calibration(int butpin ){
  unsigned long timein, timeout;
  bool pressed = false;
  timein = millis();
  timeout = 5000;
  clear();
  outln("press again to calibrate\n\n");
  while( timeout > ( millis() - timein )){
    delay(1000);
    if(Serial.available())
      break;
    if (digitalRead( butpin ) == LOW){
      pressed = true;
      break;
    }
  }
  if( pressed ){
    pressed = false;
    timein = millis();
    while( timeout > ( millis() - timein )){
      delay(1000);
      if (digitalRead( butpin ) == LOW){
        pressed = true;
        break;
      }
    }
  }
  if( pressed ){
      outln("OK, going calibrate joystick now!");
      delay(2000);
      if( !calibrate() ){
        outln("Error calibration. Set default values.");
        begin();
      }else{
        outln("Calibration success! Saving data...");
        delay(2000);
        clear();
        //save_calibration_data();
        is_started = true;
      }
    }else
      begin();
}
void JoyMove::update(){
  if( ! is_started )
    return;
  read_joy_calibrate_MIN_to_MAX();
  //print_stats(0,0);
  if( joy.CX <= JOY_MIN && joy.CY <= JOY_MIN )
    thedrive->stopMoving();
  else{
    if( joy.dirX == JOY_DIR_LEFT )
      if( joy.dirY == JOY_DIR_UP )
        thedrive->moveForward( joy.CX, joy.CY );
      if( joy.dirY == JOY_DIR_DOWN )
        thedrive->moveBackward ( joy.CX, joy.CY );

    if( joy.dirX == JOY_DIR_RIGHT )
      if( joy.dirY == JOY_DIR_UP )
        thedrive->moveForward( joy.CX, joy.CY );
      if( joy.dirY == JOY_DIR_DOWN )
        thedrive->moveBackward ( joy.CX, joy.CY );
  }
}
int JoyMove::get_speed_A(){
  return joy.CX;
}
int JoyMove::get_speed_B(){
  return joy.CY;
}
char *JoyMove::get_txt_speed_A(){
  txtA[0]='A'; txtA[1]=':';
  strncpy( txtA+2, itoa( joy.CX, bufint, 10 ), 6);
  return txtA;
}
char *JoyMove::get_txt_speed_B(){
  txtB[0]='B'; txtB[1]=':';
  strncpy( txtB+2, itoa( joy.CY, bufint, 10 ), 6);
  return txtB;
}
char *JoyMove::get_txt_direction_X(){
  if( joy.CX <= JOY_MIN ){
    strcpy( txtA, "Direction X:CENTER" );
    return txtA;
  }else strcpy( txtA, "Direction X:" );
  if( joy.dirX == JOY_DIR_LEFT )
    strncpy( txtA+12, "LEFT", 5);
  if( joy.dirX == JOY_DIR_RIGHT )
    strncpy( txtA+12, "RIGHT", 6);
  return txtA;
}
char *JoyMove::get_txt_direction_Y(){
  if( joy.CY <= JOY_MIN ){
    strcpy( txtB, "Direction Y:CENTER" );
    return txtB;
  }else strcpy( txtB, "Direction Y:" );
  if( joy.dirY == JOY_DIR_UP )
    strncpy( txtB+12, "UP", 3);
  if( joy.dirY == JOY_DIR_DOWN )
    strncpy( txtB+12, "DOWN", 5);
  return txtB;
}

void JoyMove::read_JOY(){
  float fx = analogRead(joy.pinX)/5;
  float fy = analogRead(joy.pinY)/5;
  int X = fx*5;
  int Y = fy*5;

  //avoiding little moving
  int roundX_A = joy.cX - JOY_APPROX_ROUND_CENTER_JOYSTICK;
  int roundX_B = joy.cX + JOY_APPROX_ROUND_CENTER_JOYSTICK;
  int roundY_A = joy.cY - JOY_APPROX_ROUND_CENTER_JOYSTICK;
  int roundY_B = joy.cY + JOY_APPROX_ROUND_CENTER_JOYSTICK;

  joy.X = ( X > roundX_A && X < roundX_B ) ? joy.cX : X;
  joy.Y = ( Y > roundY_A && Y < roundY_B ) ? joy.cY : Y;

  joy.dirX = ( joy.X <= joy.cX ) ? JOY_DIR_RIGHT : joy.dirX;
  joy.dirX = ( joy.X > joy.cX ) ? JOY_DIR_LEFT : joy.dirX;
  joy.dirY = ( joy.Y <= joy.cY ) ? JOY_DIR_DOWN : joy.dirY;
  joy.dirY = ( joy.Y > joy.cY ) ? JOY_DIR_UP : joy.dirY;
}
void JoyMove::read_joy_calibrate_MIN_to_MAX(){
  read_JOY();
  if( joy.dirX == JOY_DIR_RIGHT )
    joy.CX = map( joy.X, joy.mX, joy.cX, JOY_MAX, JOY_MIN );
  if( joy.dirX == JOY_DIR_LEFT )
    joy.CX = map( joy.X, joy.cX, joy.MX, JOY_MIN, JOY_MAX );
  if( joy.dirY == JOY_DIR_DOWN )
    joy.CY = map( joy.Y, joy.mY, joy.cY, JOY_MAX, JOY_MIN );
  if( joy.dirY == JOY_DIR_UP )
    joy.CY = map( joy.Y, joy.cY, joy.MY, JOY_MIN, JOY_MAX );
}
bool JoyMove::calibrate(){
  unsigned long timenow;
  float fx,fy;
  int X,Y;

  outln("Center the joystick and wait end center-calibration (2 seconds)");
  delay(2000);
  outln("started...");

  timenow = millis();
  while( 2000 > ( millis() - timenow )){
    fx = analogRead(joy.pinX)/5;
    fy = analogRead(joy.pinY)/5;
    X = fx*5;
    Y = fy*5;
    joy.cX = joy.mX = joy.MX = X;
    joy.cY = joy.mY = joy.MY = Y;
  }
  outln("done.");

  outln("Make 2 rotation for 360° the joystick and wait end calibration (3 seconds)");
  delay(2000);
  outln("started...");

  timenow = millis();
  while( 3000 > ( millis() - timenow )){
    fx = analogRead(joy.pinX)/5;
    fy = analogRead(joy.pinY)/5;
    X = fx*5;
    Y = fy*5;
    joy.mX = ( X < joy.mX ? X : joy.mX );
    joy.mY = ( Y < joy.mY ? Y : joy.mY );
    joy.MX = ( X > joy.MX ? X : joy.MX );
    joy.MY = ( Y > joy.MY ? Y : joy.MY );
  }
  outln("done.");
  if( joy.cX <= 0 || joy.mX == joy.cX || joy.MX == joy.mX || joy.MX == joy.cX )
    return false;
  return true;
}
void JoyMove::print_stats(int m1, int m2 ){
return;
  char s[]="                                                                                                                         ";
  sprintf(s, "joy.X:%d\tjoy.Y:%d\tcX:%d\tcY:%d\tmX:%d\tmY:%d\tMX:%d\tMY%d\tCX:%d\tCY:%d", joy.X, joy.Y, joy.cX, joy.cY, joy.mX, joy.mY, joy.MX, joy.MY, joy.CX, joy.CY );
  outln( s );
}
void JoyMove::out(String s){
  //Serial.print(s);
  oled->push(s);
}
void JoyMove::outln(String s){
  s += "\r\n";
  //Serial.println(s);
  oled->push(s);
}
void JoyMove::ready(){
  outln("\n\n \n\n \n\n \n\n     READY\n\n");
}
void JoyMove::clear(){
  outln(" \n\n \n\n \n\n \n\n \n\n \n\n \n\n \n\n");
}