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
#include "LolinOled.h"

LolinOled::LolinOled(Adafruit_SSD1306 *dis){
  cnt_row = 0;
  cnt_char = 0;
  cols = 21;
  rows = 8;
  //allocation of text buffer array
  for(int i=0; i<rows; i++){
    textbuf[i] = (char *)malloc(cols + 1); //last is EOL end of line
    textbuf[i][0]='\0'; //put EOL char at begin of string, empty string
  }
  disp = dis;
}
void LolinOled::start(){
  disp->begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  disp->display();
  delay(2000); // Pause for 2 seconds
  disp->clearDisplay();
  //prepare display
  disp->setTextSize(1);
  disp->setTextColor(WHITE);
  disp->setCursor(0,0);
}
void LolinOled::flush(){
  disp->flush();
  disp->display();
}
void LolinOled::shiftUp(){ //scroll to up
  for(int i=0;i<(rows-1);i++) //copy any line to previous line
    sprintf( textbuf[i], "%.*s", strlen(textbuf[i+1]), textbuf[i+1] );
}
void LolinOled::writeTextBuf(){ //send to display
  for(int i=0;i<rows;i++){
    disp->setCursor( 0, 8*i ); //position cursor on calculated y on display
    disp->println( textbuf[i] );
  }
  disp->display();
}
void LolinOled::push(String str) {
  char c = '\0';
  int i = 0, l = str.length();
  String readString="";
  //polling from str
  while ( i < l) {
    c = str.charAt( i++ );
    if( c == '\r' ) continue; //drop this special char
    if( c != '\n' ){ //adding c while lenght of temp buffer is minor than cols
      readString += c;
      cnt_char++;
    }
    //if temp buffer lenght is minor of cols and last char is newline
    //OR char counter is equal to cols
    //then line is ready to go
    if( c == '\n' || cnt_char == cols ){
      disp->setTextColor( BLACK, BLACK ); //black on black simulate deleting old line
      writeTextBuf();
      if( ++cnt_row == rows ){ //last line is gone, then scroll all to up
        shiftUp();
        cnt_row--; //now a next line is more available
      }
      //send temp buffer to text display buffer
      snprintf( textbuf[cnt_row], readString.length()+1, "%s\n", readString.c_str() );
      //now text is written white on black
      disp->setTextColor( WHITE, BLACK );
      writeTextBuf();
      //reset line
      readString="";
      cnt_char = 0;
    }
  }
}
void LolinOled::writeAtRow(char *txt, int r){
  disp->setTextColor( BLACK, BLACK ); //black on black simulate deleting old line
  disp->setCursor( 0, 8*r );
  disp->print( textbuf[r] );
  disp->setTextColor( WHITE, BLACK );
  disp->setCursor( 0, 8*r );
  disp->print( txt );
  disp->display();
  strncpy( textbuf[r], txt, cols );
}
void LolinOled::drawSemiBarLeft(int row, int vx, int min, int max){
  int px = map( vx, min, max, 0, SCREEN_WIDTH/2 );
  disp->fillRect( SCREEN_WIDTH/2-px, row*8, px, 8, WHITE );
  disp->fillRect( 0, row*8, SCREEN_WIDTH/2-px, 8, BLACK );
}
void LolinOled::drawSemiBarRight(int row, int vx, int min, int max){
  int px = map( vx, min, max, 0, SCREEN_WIDTH/2 );
  disp->fillRect( SCREEN_WIDTH/2, row*8, px, 8, WHITE );
  disp->fillRect( SCREEN_WIDTH/2+px, row*8, SCREEN_WIDTH/2-px, 8, BLACK );
}

