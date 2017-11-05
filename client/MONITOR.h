#ifndef MONITOR_H
#define MONITOR_H

#define maxStation 64

#include"SCAN.h"
#include"MAC.h"
#include"STRUCTS.h"

extern "C" {
#include "user_interface.h"
}
extern SCAN scan;

class MONITOR {
  private:
    MAC receiver, transmitter, bssidAP;
    MAC boardcast, multicast, unknown1, unknown2;
    bool selected[maxStation];
    int numBeacon, numDeauth, numDisasso;
    int curTime;
    int timeout;
    String results;
    int flag;
  public:
    MACLIST stationList;
    int numOfKnownStation;
    MONITOR();
    void startBad(int timeout);
    void badGuy(uint8_t  *buff, uint16_t len);
    void parseHeaderBad(macHeader &header);
    void startGood(int timeout, int channel);
    void goodGuy(uint8_t  *buff, uint16_t len);
    void parseHeaderGood(macHeader &header);
    bool registeredStation(MAC& station);
    void clearAll();
    void setSelected(int i, int flag=0);
    bool getSelected(int i);
    void printStation();
    void printSelected();
    void sendBadResults();
    void sendGoodResults();
};
#endif

