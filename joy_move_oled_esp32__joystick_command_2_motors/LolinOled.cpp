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
  set_cols(COLUMNS);
  set_rows(ROWS);
  //allocation of text buffer array
  for(int i=0; i<get_rows(); i++){
    textbuf[i] = (char *)malloc(get_cols() + 1); //last is EOL end of line
    textbuf[i][0] = '\0'; //put EOL char at begin of string, empty string
//    textbuf_style[i] = Straigh;
    textbuf_color[i] = Normal;
  }
  disp = dis;
  cursorY=0;
//  textbuf_style[1]=Bold;
}
void LolinOled::set_rows(int r){
  rows=r;
}
void LolinOled::set_cols(int c){
  cols=c;
}
int LolinOled::get_rows(){
  return rows;
}
int LolinOled::get_cols(){
  return cols;
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
  for(int i=0;i<(get_rows()-1);i++) //copy any line to previous line
    sprintf( textbuf[i], "%.*s", strlen(textbuf[i+1]), textbuf[i+1] );
}
void LolinOled::writeTextBuf(TEXT_OVER textover){ //send to display
  for(int i=0;i<get_rows();i++){
    disp->setCursor( 0, 8*i ); //position cursor on calculated y on display
    if( textover )
      disp->setTextColor( BLACK, BLACK ); //black on black simulate deleting old line
    else
      set_color_font(i);
    disp->println( textbuf[i] );
  }
  disp->display();
}
void LolinOled::refresh(){
  writeTextBuf(ERASE);
  writeTextBuf(STYLE);
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
    if( c == '\n' || cnt_char == get_cols() ){
      writeTextBuf(ERASE);
      if( ++cnt_row == get_rows() ){ //last line is gone, then scroll all to up
        shiftUp();
        cnt_row--; //now a next line is more available
      }
      //send temp buffer to text display buffer
      snprintf( textbuf[cnt_row], readString.length()+1, "%s\n", readString.c_str() );
      //now text is written white on black
      writeTextBuf(STYLE);
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
  set_color_font(r);//read color value from row r of textbuf_color[] and set colors on display
//  set_style_font(r);;//read style value from row r of textbuf_style[] and set style on display
  disp->setCursor( 0, 8*r );
  disp->print( txt );
  disp->display();
  strncpy( textbuf[r], txt, get_cols() );
}
void LolinOled::set_color_textbuf_on_cursor( TEXT_COLORS cl ){
  textbuf_color[cursorY]=cl;
}
void LolinOled::set_cursor_position(int y){
  cursorY = y;
}
void LolinOled::set_color_font( int r ){
  switch( textbuf_color[r] ){
    case Normal: disp->setTextColor( WHITE, BLACK ); break;
    case Evidence: disp->setTextColor( BLACK, WHITE ); break;
    default: break;
  }
}
/*//style font family not used because is not useful and unstable
void LolinOled::set_style_textbuf_on_cursor( TEXT_STYLES ts ){
  textbuf_style[cursorY]=ts;
}
void LolinOled::set_style_font( int r ){
  switch( textbuf_style[r] ){
    case Straigh: disp->setFont(); break;
    case Bold: disp->setFont(); break;
    default: break;
  }
}*/
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
int LolinOled::cursorUp(){
  set_color_textbuf_on_cursor(oldcolor);//restore prec color
  if( --cursorY < 0 )
    cursorY = (num_of_items - 1);
  oldcolor=textbuf_color[cursorY];//save this color
  set_color_textbuf_on_cursor(Evidence);
  refresh();
  return cursorY;
}
int LolinOled::cursorDown(){
  set_color_textbuf_on_cursor(oldcolor);
  if( ++cursorY >= num_of_items )
    cursorY = 0;
  oldcolor=textbuf_color[cursorY];
  set_color_textbuf_on_cursor(Evidence);
  refresh();
  return cursorY;
}
void LolinOled::cursorOn(int noi){
  if( noi > get_rows() )
    num_of_items = get_rows();
  else
    num_of_items = noi;
  set_cursor_position( 0 );
  set_color_textbuf_on_cursor(oldcolor);
  oldcolor=textbuf_color[cursorY];
  set_color_textbuf_on_cursor(Evidence);
  refresh();
}
void LolinOled::cursorBlink(){
  set_color_textbuf_on_cursor(oldcolor);
  writeAtRow(textbuf[cursorY], cursorY);
  delay(250);
  oldcolor=textbuf_color[cursorY];
  set_color_textbuf_on_cursor(Evidence);
  writeAtRow(textbuf[cursorY], cursorY);
}
void LolinOled::cursorStop(){
  set_color_textbuf_on_cursor(oldcolor);
  disp->clearDisplay();
}
void LolinOled::get_textbuf(char *txt,int r){
  if(r>0 && r<get_rows())
    strncpy( txt, textbuf[r], get_cols() );
}
void LolinOled::set_textbuf(char *txt,int r){
  if(r>0 && r<get_rows())
    strncpy( textbuf[r], txt, get_cols() );
}
int LolinOled::get_row_selected(){
  return cursorY;
}
void LolinOled::circle_on_text_row(int r){
  disp->fillCircle(124, 8*r+4, 2, WHITE );
  disp->drawLine(110, 8*r+4, 124, 8*r+4, WHITE);
}