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
#include "Drive.h"
#include "JoyMove.h"

#include "LolinOled.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define JOYSTICK_X_PIN      0 //so boot button is unusable, to use it change pin to 12,13,14,15
#define JOYSTICK_Y_PIN      2

#define OLED_SDA_PIN 5
#define OLED_SCL_PIN 4

#define BUTTON_PIN 16

//Define L298N pin mappings (the two jumpers are plugged in)
const int IN1 = 15;
const int IN2 = 13;
const int IN3 = 12;
const int IN4 = 14;

//minimum of 12V znd also gives out 5V to power the board
Drive drive(IN1, IN2, IN3, IN4);

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
LolinOled loled( &display );

JoyMove joymove(JOYSTICK_X_PIN, JOYSTICK_Y_PIN, &drive, &loled );

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db); // Set the ADC attenuation to 11 dB (up to ~3.3V input)
  Wire.begin( OLED_SDA_PIN, OLED_SCL_PIN ); // Initialize I2C

  loled.start();
  joymove.start();
  delay(1000);
  joymove.ready();
}
unsigned long timep_in=0, timep_out=100; //refresh time display in milliseconds
void loop() {
  if(timep_in == 0)
    timep_in = millis();
  if( timep_out < millis() - timep_in ){
    timep_in = millis();
    loled.writeAtRow( joymove.get_txt_speed_A(), 0 );
    loled.writeAtRow( joymove.get_txt_speed_B(), 1 );
  }
  joymove.update();
  if (digitalRead(BUTTON_PIN) == LOW)
    joymove.calibration( BUTTON_PIN );
}
