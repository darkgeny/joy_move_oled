/*modified by G.Catanzariti 01/05/2025*/
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
