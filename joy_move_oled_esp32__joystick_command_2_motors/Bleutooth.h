// Author: Giacomo Catanzariti
//at 9/5/2025
#ifndef Bleutooth_H
#define Bleutooth_H
#include "Arduino.h"
#include <BluetoothSerial.h>
//
#define MAX_DEVICES_DISCOVER 8
#define BT_DEV_NAME_MAX_LEN 248
#define MACADDRESS_LENGTH 18
#define BT_NAME_PREFIX "JIMIBT-"

#define CONNECTED true
#define DISCONNECTED false

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

#define BT_DISCOVER_TIME 3000

struct DEVICES_BLEUTOOTH {
  char *addr;
  char *name;  //two dimensional array of char
  bool is_connected;
};
typedef struct DEVICES_BLEUTOOTH BLEU_DEVICE;

class Bleutooth {
  public:
    Bleutooth();
    Bleutooth(BluetoothSerial *);
    void discovery();
    char **get_list_discovery();
    void show_dev_list();
    void push_new_device(const char *name, const char *addr);
    bool connect(int);
    int get_devnum();
    DEVICES_BLEUTOOTH get_device(int);
    //int get_first_with_prefix(const char *);
    void set_is_connected(int);
    void set_is_NOT_connected(int);
  private:
    void inc_devnum();
    unsigned int devnum;
    DEVICES_BLEUTOOTH *device;
    BluetoothSerial *serbt;
    void clear();
};
#endif