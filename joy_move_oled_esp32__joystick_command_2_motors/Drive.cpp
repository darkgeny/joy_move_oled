/*
  Drive.h - Library for controlling Robot Car with L298x bridge series
  Created by Oladapo E. Ajala, November 13, 2018.
  Released into the public domain.
  Copy right InvenTech llc.
*/
/*modified by G.Catanzariti 01/05/2025*/
#include "Arduino.h"
#include "Drive.h"

#define LOW 0

Drive::Drive(int IN1, int IN2, int IN3, int IN4)
{
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  _IN1 = IN1;
  _IN2 = IN2;
  _IN3 = IN3;
  _IN4 = IN4;
}

bool Drive::moveForward(int speedA, int speedB)
{
  analogWrite(_IN1, speedA);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, speedB);
  analogWrite(_IN4, LOW);  
  return true;
}

bool Drive::moveBackward(int speedA, int speedB)
{
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, speedA);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, speedB); 
  return true;
}

bool Drive::turnLeft(int speedA, int speedB)
{
  analogWrite(_IN1, speedA);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, speedB);
  return true;
}

bool Drive::turnRight(int speedA, int speedB)
{
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, speedA);
  analogWrite(_IN3, speedB);
  analogWrite(_IN4, LOW);
  return true;  
}

bool Drive::stopMoving()
{
  analogWrite(_IN1, LOW);
  analogWrite(_IN2, LOW);
  analogWrite(_IN3, LOW);
  analogWrite(_IN4, LOW); 
  return true;  
}
