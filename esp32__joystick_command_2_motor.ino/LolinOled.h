/* by G.Catanzariti 07/05/2025*/
#ifndef LolinOled_h
#define LolinOled_h
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display TWI address
#define OLED_ADDR   0x3C
// Reset pin not used on 4-pin OLED module
//Adafruit_SSD1306 display(-1);

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Initialize with the I2C addr 0x3C (for the 128x64)
// Always reset the display at the beginning

#include "Arduino.h"

class LolinOled
{
  public:
    LolinOled(Adafruit_SSD1306 *);
    void start();
    void flush();
    void push(String);
    void shiftUp();
    void writeTextBuf();
    void writeAtRow(char *, int);
  private:
    int cnt_row; //row counter
    int cnt_char; //col counter
    int cols; //maximum cols of oled display
    int rows; //maximum rows of oled display
    char *textbuf[8]; //two dimensional array of char
    Adafruit_SSD1306 *disp;
};
#endif