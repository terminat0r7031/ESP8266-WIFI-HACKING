#ifndef SCAN_h
#define SCAN_h
#include<Arduino.h>
#include<ESP8266WiFi.h>
#include"MAC.h"
#include"MACLIST.h"
//#include<ESP8266WebServer.h>
//extern ESP8266WebServer server;
//extern void sendBuffer();
//extern void sendToBuffer(String str);
//extern void sendHeader(int code, String type, size_t _size);

class SCAN {
  private:
    String* essid;
    int* channel;
    int* rssi;
    uint8_t* encryption;
    bool* hidden;
    bool* selected;
    int numSelected;
  public:
    const int maxNumOfNetwork;
    int numOfNetwork;
    MACLIST bssid;
    SCAN();
    ~SCAN();
    void start();
    String getEssid(int i);
    String getBssid(int i);
    int getChannel(int i);
    int getRssi(int i);
    String getEncryption(int i);
    bool  getHidden(int i);
    bool  getSelected(int i);
    void  setSelected(int i, int flag=0);
    void  printResults();
    void  sortByRssi();
    void  sendResults();
    int numOfAPSelected();
    void printSelected();
    void parseEssid();
};
#endif

