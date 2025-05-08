/* by G.Catanzariti 07/05/2025*/
#ifndef JoyMove_h
#define JoyMove_h

#include "Arduino.h"
#include "Drive.h"
#include "LolinOled.h"

#define JOY_APPROX_ROUND_CENTER_JOYSTICK 100
#define JOY_DIR_LEFT 0
#define JOY_DIR_RIGHT 1
#define JOY_DIR_UP 2
#define JOY_DIR_DOWN 3
#define JOY_MIN 300
#define JOY_MAX 1023

#define PRESET_CENTER_X 2640
#define PRESET_CENTER_Y 1820
#define PRESET_MIN 0
#define PRESET_MAX 4095

class JoyMove
{
  public:
    JoyMove(int, int, Drive *, LolinOled *);
    void begin();
    void begin(int, int);
    void update();
    int get_speed_A();
    int get_speed_B();
    char *get_txt_speed_A();
    char *get_txt_speed_B();
    void calibration(int);
    void start();
    void out(String);
    void outln(String);
    void ready();
    void clear();
  private:
    Drive *thedrive;
    bool calibrate();
    struct JOYSTICK_DATA{
      int pinX,pinY; //pins
      int X,Y; //direct values
      int cX,cY; //centered
      int mX,mY; //minimum
      int MX,MY; //maximum
      int dirX,dirY; //directions
      int CX,CY; //calibrated
    };
    typedef struct JOYSTICK_DATA JOY;
    JOY joy;
    void print_stats(int, int );
    void read_joy_calibrate_MIN_to_MAX();
    void read_JOY();
    //void delayP(int sec);
    void save_calibration_data();
    void load_calibration_data();
    bool is_started;
    LolinOled *oled;
    char *txtA, *txtB, *bufint;
};
#endif