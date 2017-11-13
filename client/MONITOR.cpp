#include"MONITOR.h"
MONITOR::MONITOR() {
  boardcast = MAC(0xff, 0xff, 0xff, 0xff, 0xff, 0xff);
  multicast = MAC(0x01, 0x00, 0x5e, 0x7f, 0xff, 0xfa);
  unknown1 = MAC(0x01, 0x00, 0x5e, 0x00, 0x00, 0x00);
  unknown2 = MAC(0x33, 0x33, 0x00, 0x00, 0x00, 0x00);
  numOfKnownStation = 0;
  for (int i = 0; i < maxStation; i++)
    selected[i] = false;
}
void MONITOR::clearAll() {
  stationList.clearAll();
  setSelected(-2, 1);
  numOfKnownStation = 0;
}
void MONITOR::startBad(int timeout) {
  if (scan.numOfAPSelected() == 1) {
    clearAll();
    int channelAP;
    curTime = millis();
    (*this).timeout = timeout;
    for (int i = 0; i < scan.numOfNetwork && i < scan.maxNumOfNetwork; i++) {
      if (scan.getSelected(i) == true) {
        bssidAP = scan.bssid._get(i);
        channelAP = scan.getChannel(i);
        break;
      }
    }
    //  bssidAP = MAC(0xa0,0x65,0x18,0xab,0x48,0xc3);
    //  channelAP = 6;
    wifi_promiscuous_enable(0);
    wifi_set_opmode(STATION_MODE);
    wifi_set_channel(channelAP);
    wifi_promiscuous_enable(1);
  }
}
void stop() {
  wifi_promiscuous_enable(0);
  digitalWrite(BUILTIN_LED, HIGH);
}
void MONITOR::badGuy(uint8_t *buff, uint16_t len) {
  if (millis() - curTime > timeout * 1000) {
    Serial.println("RS_S_S_S");
    stop();
  }
  if (len > 12) {
    struct snifferBuff *captured = (struct snifferBuff*) buff;
    parseHeaderBad(captured->header);
  }
}
void MONITOR::parseHeaderBad(macHeader &header) {
  receiver = MAC(header.addr1);
  transmitter = MAC(header.addr2);
  if (receiver != boardcast) {
    if (receiver == bssidAP) {
      if ((transmitter.compare3Byte(unknown1)) || (transmitter.compare3Byte(unknown2)));
      else {
        if (!registeredStation(transmitter)) {
          if (numOfKnownStation < maxStation) {
            stationList.add(transmitter);
            numOfKnownStation++;
          }
        }
      }
    }
    if (transmitter == bssidAP) {
      if ((receiver.compare3Byte(unknown1)) || (receiver.compare3Byte(unknown2)));
      else {
        if (!registeredStation(receiver)) {
          if (numOfKnownStation < maxStation) {
            stationList.add(receiver);
            numOfKnownStation++;
          }
        }
      }
    }
  }
}
bool MONITOR::registeredStation(MAC& station) {
  for (int i = 0; i < numOfKnownStation; i++) {
    if (station == stationList._get(i)) {
      return true;
    }
  }
  return false;
}
void MONITOR::setSelected(int i, int flag) {
  switch (i) {
    case -1: { //select all
        for (int j = 0; j < numOfKnownStation; j++) {
          selected[j] = true;
        } break;
      }
    case -2: { //deselect all
        for (int j = 0; j < numOfKnownStation; j++) {
          selected[j] = false;
        }
      } break;
    default: {
        if (i < numOfKnownStation && i < maxStation) {
          if (selected[i] == true) {
            selected[i] = false;
          }
          else {
            selected[i] = true;
          }
        }
      }
  }
  if (flag == 0) Serial.println("RS_SL_ST");
}
bool MONITOR::getSelected(int i) {
  if (i < numOfKnownStation && i < maxStation) {
    return selected[i];
  }
}
void MONITOR::printStation() {
  for (int i = 0; i < numOfKnownStation; i++) {
    Serial.println(stationList._get(i).toString());
  }
}
void MONITOR::printSelected() {
  for (int i = 0; i < numOfKnownStation; i++) {
    if (selected[i] == true) {
      Serial.println(stationList._get(i).toString());
    }
  }
}
void MONITOR::startGood(int timeout, int channel) {
  curTime = millis();
  (*this).timeout = timeout;
  numBeacon = 0;
  numDeauth = 0;
  numDisasso = 0;
  wifi_promiscuous_enable(0);
  wifi_set_opmode(STATION_MODE);
  wifi_set_channel(channel);
  wifi_promiscuous_enable(1);
  flag = 0;
}
void MONITOR::goodGuy(uint8_t * buff, uint16_t len) {
  if (millis() - curTime > timeout * 1000) {
    Serial.println("RS2_MON");
    stop();
  }
  if ((millis() - curTime) % 2000 < 100) {
    if (flag == 0) {
      sendGoodResults();
      numBeacon = 0;
      numDeauth = 0;
      numDisasso = 0;
      flag = 1;
    };
  }
  else {
    flag = 0;
  }
  if (len > 12) {
    struct snifferBuff *captured = (struct snifferBuff*) buff;
    parseHeaderGood(captured->header);
  }
}
void MONITOR::parseHeaderGood(macHeader& header) {
  int fc =  (header.fc & 0xff) >> 4;
  switch (fc) {
    case 8: { // beacon frame
        numBeacon++;
      } break;
    case 10: {
        numDisasso++;
      } break;
    case 12: {
        numDeauth++;
      } break;
  }
}
void MONITOR::sendBadResults() {
  /*
     JSON template
          json = {
            "bssid":"00:11:22:33:44:55",
            "station":["11:22:33:44:55:66","22:33:44:55:66:77"]
          }

  */
  size_t _size = 42;       // {"bssid":"00:11:22:33:44:55","station":[]}
  for (int i = 0; i < numOfKnownStation; i++) {
    _size += 20;          // "00:11:22:33:44:55",
  }
  _size--;                 // trừ đi dấu ',' của phần tử cuối
  String json;
  json += "{\"bssid\":\"";
  json += bssidAP.toString();
  json += "\",\"station\":[";
  for (int i = 0; i < numOfKnownStation; i++) {
    json += "\"";
    json += stationList._get(i).toString();
    json += "\"";
    if (i < numOfKnownStation - 1) {
      json += ",";
    }
  }
  json += "]}";
  int numPart = (int) (_size / 99) + 1;
  String tmp = "";
  while (json != "") {
    tmp += "RS_S_ST_R ";
    tmp += numPart;
    tmp += " ";
    tmp += json.substring(0, 99);
    json = json.substring(99);
    Serial.println(tmp);
    delay(750);
    tmp = "";
  }
}
void MONITOR::sendGoodResults() {
  /*
        JSON template
        json = {
            "bc":"12",
            "de":"1",
            "dis":"2"
        }
  */
  size_t _size = 26;   //{"bc":"","de":"","dis":""}
  _size += String(numBeacon).length();
  _size += String(numDeauth).length();
  _size += String(numDisasso).length();

  String json;
  json += "{\"bc\":\"";
  json += String(numBeacon);
  json += "\",\"de\":\"";
  json += String(numDeauth);
  json += "\",\"dis\":\"";
  json += String(numDisasso);
  json += "\"}";

  String cmd = "RS1_MON ";
  cmd += json;
  Serial.println(cmd);
}
