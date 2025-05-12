// Author: Giacomo Catanzariti
//at 9/5/2025
//Menu is not yet implemented on joy_move_oled
#ifndef MENU_H
#define MENU_H
#include "Arduino.h"
#include "Button.h"
#include "WebLogging.h"
#include "JoyMove.h"
#include "LolinOled.h"
#include "Bleutooth.h"

#define CONFIRMED true
#define ABORTED false

//
class Menu{
  public:
    Menu();
    Menu(JoyMove *, LolinOled *, Button *, Bleutooth*, WebLogging *);
    void main_page();
  private:
    void confirm_selected_device();
    bool scan_for_devices();
    void bluetooth_show_menu_devices();
    JoyMove *joymove;
    LolinOled *loled;
    Button *button;
    WebLogging *wlog;
    unsigned long timep_in, timep_out; //refresh time display in milliseconds
    bool getready;
    bool is_moved_to(int);
    int move_up_down();
    int move_left_right();
    bool is_move_back();
    bool is_move_forward();
    void wait_recenteredX();
    void wait_recenteredY();
    char *old_textbuf;
    Bleutooth *bleut;
    char *linea;//temp row string
};
#endif