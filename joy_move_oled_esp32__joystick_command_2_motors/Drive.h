/*
  Drive.h - Library for controlling Robot Car with L298x bridge series
  Created by Oladapo E. Ajala, November 13, 2018.
  Released into the public domain.
  Copy right InvenTech inc.
*/
/*modified by G.Catanzariti 01/05/2025*/
#ifndef Drive_h
#define Drive_h

#include "Arduino.h"

class Drive
{
  public:
    Drive(int IN1, int IN2, int IN3, int IN4);
    bool moveForward(int, int);
    bool moveBackward(int, int);
    bool turnRight(int, int);
    bool turnLeft(int, int);
    bool stopMoving();
  private:
	int _IN1;
	int	_IN2;
	int _IN3;
	int _IN4;
};

#endif
