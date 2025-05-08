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