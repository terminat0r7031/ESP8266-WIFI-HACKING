#ifndef BEACON_H
#define BEACON_H
#include<Arduino.h>
#include"MAC.h"
extern "C" {
#include "user_interface.h"
}
class BEACON {
  private:
    uint8_t packet[128] = {

      /* MAC header */
      /* 0 - 1 */     0x80, 0x00, //Frame Control
      /* 2 - 3 */     0x00, 0x00, //Duration
      /* 4 - 9 */     0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address
      /* 10 - 15 */   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address
      /* 16 - 21 */   0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID
      /* 22 - 23 */   0xc0, 0x6c, //Sequence number

      /* Frame body */
      /* Fixed parameters */
      /* 24 - 31 */   0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp
      /* 32 - 33 */   0x64, 0x00, //beacon interval
      /* 34 - 35 */   0x01, 0x04, //capability info

      /* Tagged parameters */
      /* SSID */
      /* 36 */        0x00,  //Tag Number: SSID Parameter Set (0)
    };

    uint8_t remain[35] = {
      /* supported rate */
      0x01, //Tag Number: Supported Rates (1)
      0x08, //Tag length: 8
      0x82, //  1Mbps (Basic)
      0x84, //  2Mbps (Basic)
      0x8b, //  5.5Mbps (Basic)
      0x96, //  11Mbps (Basic)
      0x12, //  9Mbps
      0x24, //  18Mbps
      0x48, //  36Mbps
      0x6c, //  54Mbps

      /*  DSSS (Current Channel) */
      0x03, //Tag Number: DS Parameter Set (3)
      0x01, //Tag length: 1
      0x0a, //Current channel: 10

      /* RSN Information */
      0x30,                   //Tag Number: RSN Information (48)
      0x14,                   //Tag length: 20
      0x01, 0x00,             //RSN Version: 1
      //Group Cipher Suite: 00-0f-ac AES (CCM)
      0x00, 0x0f, 0xac,     //Group Cipher Suite OUI: 00-0f-ac
      0x04,                 //Group Cipher Suite Type: AES (CCM) (4)
      0x01, 0x00,             //Pairwise Cipher Suite Count: 1
      //Pairwise Cipher Suite List 00-0f-ac AES (CCM)
      //Parwise Cipher Suite: 00-0f-ac AES (CCM)
      0x00, 0x0f, 0xac, //Pairwise Cipher Suite OUI: 00-0f-ac (CCM)
      0x04,             //Pairwise Cipher Suite type: AES (CCM) (4)
      0x01, 0x00,             //Auth Key Management (AKM) Suite Count: 1
      //Auth Key Management (AKM) List 00-0f-ac PSK
      //Auth Key Management (AKM) Suite: 00-0f-ac PSK
      0x00, 0x0f, 0xac, //Auth Key Management (AKM) OUI: 00-0f-ac
      0x02,             //Auth Key Management (AKM) type: PSK (2)
      //RSN Capabilities
      0x00, 0x00

    };
    int packetSize;
    bool send();
    void build(String& ssid);
  public:
    BEACON();
    void attack(int timeout); //random SSID
    void attack(String& tssid, int timeout);
    String randomSSID();
    void start();
    void stop();
};
#endif
