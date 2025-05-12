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
#define COLUMNS 21
#define ROWS 8

// Initialize with the I2C addr 0x3C (for the 128x64)
// Always reset the display at the beginning

#include "Arduino.h"

//style font family not used because is not useful and unstable
//enum TEXT_STYLES { Straigh, Bold };
enum TEXT_COLORS { Normal, Evidence };
enum TEXT_OVER { STYLE, ERASE };

class LolinOled
{
  public:
    LolinOled(Adafruit_SSD1306 *);
    void start();
    void flush();
    void push(String);
    void shiftUp();
    void writeTextBuf(TEXT_OVER);
    void get_textbuf(char *,int);
    void set_textbuf(char *,int);
    void refresh();
    void writeAtRow(char *, int);
    void set_rows(int);
    void set_cols(int);
    int get_rows();
    int get_cols();
    void drawSemiBarLeft(int, int, int, int);
    void drawSemiBarRight(int, int, int, int);
    int cursorUp();
    int cursorDown();
    void cursorStop();
    void cursorOn(int);
    void cursorBlink();
    int get_row_selected();
    void circle_on_text_row(int);
  private:
    void set_cursor_position(int);
    int cnt_row; //row counter
    int cnt_char; //col counter
    int cols; //maximum cols of oled display
    int rows; //maximum rows of oled display
    char *textbuf[ROWS]; //two dimensional array of char
//    TEXT_STYLES textbuf_style[8],oldstyle;
    TEXT_COLORS textbuf_color[ROWS],oldcolor;
    void set_color_textbuf_on_cursor(TEXT_COLORS);
//    void set_style_textbuf_on_cursor(TEXT_STYLES);
    void set_color_font(int);
//    void set_style_font(int);
    int cursorY;
    int num_of_items;
    Adafruit_SSD1306 *disp;
};
#endif