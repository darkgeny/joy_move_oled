// Author: Giacomo Catanzariti
//at 9/5/2025
#include "Arduino.h"
#include "Bleutooth.h"

Bleutooth::Bleutooth(){};
Bleutooth::Bleutooth(BluetoothSerial *sbt){
  serbt = sbt;
  device = (BLEU_DEVICE *)malloc( sizeof(BLEU_DEVICE) * MAX_DEVICES_DISCOVER);
  for(int i=0; i<MAX_DEVICES_DISCOVER; i++){
    device[i].name = (char *)malloc( BT_DEV_NAME_MAX_LEN + 1);
    device[i].name[0] = '\0';
    device[i].addr = (char *)malloc( MACADDRESS_LENGTH + 1);
    device[i].addr[MACADDRESS_LENGTH] = '\0';
  }
  devnum=0;//number of devices inserted in list
}
int Bleutooth::get_devnum(){
  return devnum;
}
void Bleutooth::inc_devnum(){
  devnum++;
}
DEVICES_BLEUTOOTH Bleutooth::get_device(int n){
  return device[n];
}
void Bleutooth::push_new_device(const char *name, const char *addr){
  strncpy( device[get_devnum()].name, name, BT_DEV_NAME_MAX_LEN );
  strncpy( device[get_devnum()].addr, addr, MACADDRESS_LENGTH );
  set_is_NOT_connected(get_devnum());
  inc_devnum();
}
void Bleutooth::show_dev_list(){
  for(int i=0;i<get_devnum();i++){
    Serial.print(i);
    Serial.print(") ");
    Serial.print(device[i].name);
    Serial.print("\t");
    Serial.println(device[i].addr);
  }
}
bool Bleutooth::connect(int dev){
  return serbt->connect((BTAddress)device[dev].addr);
}
void Bleutooth::discovery(){
  devnum = 0;
  /*serbt->discoverClear();
  serbt->flush();
  serbt->disconnect();
  serbt->end();
  delay(1000);*/

  if (!serbt->begin("joymmr", true)) {
    Serial.println("========== SerialBT failed!");
    abort();
  }
  //Serial.println("Starting discoverAsync...");
  BTScanResults *btDeviceList = serbt->getScanResults();
  if (serbt->discoverAsync([](BTAdvertisedDevice *pDevice) {
        //Serial.printf(">>>>>>>>>>>Found a new device asynchronously: %s\n", pDevice->toString().c_str());
      })) {
    delay(BT_DISCOVER_TIME);
    serbt->discoverAsyncStop();
//    delay(3000);
    if (btDeviceList->getCount() > 0) {
      BTAddress addr;
      for (int i = 0; i < btDeviceList->getCount(); i++) {
        BTAdvertisedDevice *device = btDeviceList->getDevice(i);
        std::map<int, std::string> channels = serbt->getChannels(device->getAddress());
        if (channels.size() > 0) {
          addr = device->getAddress();
        }
        if (addr.toString().length())
          push_new_device(device->getName().c_str(), addr.toString().c_str());
      }
    }
  }
}
/*int Bleutooth::get_first_with_prefix(const char *pref){
  Serial.println(pref);
  for(int i=0;i<devnum;i++)
    if( strncmp( device[i].name, pref, strlen(pref)) == 0 ){
      Serial.print("first:");
      Serial.println(device[i].name);
      return i;
    }
  return -1;
}*/
void Bleutooth::set_is_connected(int ndev){
  device[ndev].is_connected = CONNECTED;
}
void Bleutooth::set_is_NOT_connected(int ndev){
  device[ndev].is_connected = DISCONNECTED;
}
