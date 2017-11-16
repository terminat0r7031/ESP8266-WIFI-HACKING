#include "SCAN.h"
SCAN::SCAN(): maxNumOfNetwork(50) {
  essid = new String [maxNumOfNetwork];
  channel = new int [maxNumOfNetwork];
  rssi = new int [maxNumOfNetwork];
  encryption = new uint8_t [maxNumOfNetwork];
  hidden = new bool [maxNumOfNetwork];
  selected = new bool [maxNumOfNetwork];
  for (int i = 0; i < maxNumOfNetwork; i++)
    selected[i] = false;
  numSelected = 0;
}
SCAN::~SCAN() {
  delete [] essid;
  delete [] channel;
  delete [] rssi;
  delete [] encryption;
  delete [] hidden;
}
void SCAN::start() {
  MAC bmac;
  bssid.clearAll();
  setSelected(-2,1);
  numOfNetwork = WiFi.scanNetworks(false, true);
  for (int i = 0; i < numOfNetwork && i < maxNumOfNetwork; i++) {
    essid[i] = WiFi.SSID(i);
    channel[i] = WiFi.channel(i);
    rssi[i] = WiFi.RSSI(i);
    encryption[i] = WiFi.encryptionType(i);
    hidden[i] = WiFi.isHidden(i);
    bmac.set(WiFi.BSSID(i)[0], WiFi.BSSID(i)[1], WiFi.BSSID(i)[2], WiFi.BSSID(i)[3], WiFi.BSSID(i)[4], WiFi.BSSID(i)[5]);
    bssid.add(bmac);
  }
  Serial.println("RS_S_S_AP");
}
String SCAN::getEssid(int i) {
  return essid[i];
}
String SCAN::getBssid(int i) {
  return bssid._get(i).toString();
}
int SCAN::getChannel(int i) {
  return channel[i];
}
int SCAN::getRssi(int i) {
  return rssi[i];
}
String SCAN::getEncryption(int i) {
  switch (encryption[i]) {
    case ENC_TYPE_NONE:
      return "None";
      break;
    case ENC_TYPE_WEP:
      return "WEP";
      break;
    case ENC_TYPE_TKIP:
      return "WPA";
      break;
    case ENC_TYPE_CCMP:
      return "WPA2";
      break;
    case ENC_TYPE_AUTO:
      return "WPA*";
      break;
  }
  return "?";
}
bool SCAN::getHidden(int i) {
  return hidden[i];
}
bool SCAN::getSelected(int i) {
  return selected[i];
}
void SCAN::printResults() {
  Serial.printf("\n/-------------ESSID------------/----------BSSID---------/-----CHANNEL----/---RSSI--/--ENCRYP--/\n");
  for (int i = 0; i < numOfNetwork && i < maxNumOfNetwork; i++) {
    if (selected[i] == true) {
      if (getHidden(i))
        Serial.printf("%32s", "*Hidden Network*");
      else
        Serial.printf("%32s", getEssid(i).c_str());
      Serial.printf("%25s", getBssid(i).c_str());
      Serial.printf("%15d", getChannel(i));
      Serial.printf("%10d", getRssi(i));
      Serial.printf("%10s", getEncryption(i).c_str());
      Serial.println();
    }
  }
}
void SCAN::sortByRssi() {
  String tmpEssid;
  int tmpChannel;
  int tmpRssi;
  uint8_t tmpEncrypt;
  bool tmpHidden;
  MAC tmpBssid;
  for (int i = 0; i < numOfNetwork; i++)
    for (int j = numOfNetwork - 1; j >= i + 1; j--) {
      if (rssi[j - 1] < rssi[j]) { //sap xep theo do manh tin hieu
        //hoan vi essid
        tmpEssid = essid[j - 1];
        essid[j - 1] = essid[j];
        essid[j] = tmpEssid;

        //hoan vi channel
        tmpChannel = channel[j - 1];
        channel[j - 1] = channel[j];
        channel[j] = tmpChannel;

        //hoan vi RSSI
        tmpRssi = rssi[j - 1];
        rssi[j - 1] = rssi[j];
        rssi[j] = tmpRssi;

        //hoan vi Encryption
        tmpEncrypt = encryption[j - 1];
        encryption[j - 1] = encryption[j];
        encryption[j] = tmpEncrypt;

        //hoan vi BSSID
        tmpBssid = bssid._get(j - 1);
        bssid._set(bssid._get(j), j - 1);
        bssid._set(tmpBssid, j);

        //hoan vi Hidden
        tmpHidden = hidden[j - 1];
        hidden[j - 1] = hidden[j];
        hidden[j] = tmpHidden;
      }
    }
}
void SCAN::sendResults() { //using json
  parseEssid();     //thay ky tu "." -> " "
  sortByRssi();     //sap xep theo RSSI
  /*
    JSON template
    json = {
     "aps" : [
        {"essid":"abc","bssid":"11:22:33:44:55:66","channel":"3","rssi":"-42","encrypt":"WPA","hidden":"false"},
        {"essid":"xyz","bssid":"aa:bb:cc:dd:ee:ff","channel":"6,"rssi":"-21","encrypt":"WPA2","hidden":"false"},
      ]
    }
  */
  size_t _size = 10; //{"aps":[]}
  for (int i = 0; i < numOfNetwork && i < maxNumOfNetwork; i++) {
    /*
      _size+=1;     // {
      _size+=5;     // "n":"
      _size+=String(getEssid(i)).length();
      _size+=2;     // ",
      _size+=23;    //  "b":"aa:bb:cc:dd:ee:ff"
      _size+=1;     // ,
      _size+=5;    // "c":"
      _size+=String(getChannel(i)).length();
      _size+=2;     // ",
      _size+=5;     // "r":"
      _size+=String(getRssi(i)).length();
      _size+=2;     // ",
      _size+=5;    // "e":"
      _size+=String(getEncryption(i)).length();
      _size+=2;     // ",
      _size+=5;    // "h":"
      _size+=String(getHidden(i)).length();
      _size+=3;     // "},
    */
    _size += 61;
    _size += getEssid(i).length();
    _size += String(getChannel(i)).length();
    _size += String(getRssi(i)).length();
    _size += getEncryption(i).length();
    _size += String(getHidden(i)).length();
  }
  _size--;
  String json;
  json = "{\"aps\":[";
  for (int i = 0; i < numOfNetwork && i < maxNumOfNetwork; i++) {
    json += "{";
    json += "\"n\":\"" + getEssid(i) + "\",";
    json += "\"b\":\"" + getBssid(i) + "\",";
    json += "\"c\":\"" + (String)getChannel(i) + "\",";
    json += "\"r\":\"" + (String)getRssi(i) + "\",";
    json += "\"e\":\"" + getEncryption(i) + "\",";
    json += "\"h\":\"" + (String)getHidden(i) + "\"}";
    if (i < numOfNetwork - 1) json += ",";
  }
  json += "]}";
  int numPart = (int) (_size/99) + 1;
  String tmp="";
  while(json!=""){
    tmp += "RS_S_AP_R/";
    tmp += numPart;
    tmp += "/";
    tmp += json.substring(0,99);
    json = json.substring(99);
    Serial.println(tmp);
    delay(750);
    tmp = "";
  }
}
void SCAN::setSelected(int i, int flag) {
  switch (i) {
    case -1: { //select all
        for (int j = 0; j < numOfNetwork && j < maxNumOfNetwork; j++) {
          selected[j] = true;
        }
        numSelected = numOfNetwork;
      } break;
    case -2: { //deselect all
        for (int j = 0; j < numOfNetwork && j < maxNumOfNetwork; j++) {
          selected[j] = false;
        }
        numSelected = 0;
      } break;
    default: { //select one
        if (i < numOfNetwork && i < maxNumOfNetwork) {
          if (selected[i] == false) {
            selected[i] = true;
            numSelected++;
          }
          else {
            selected[i] = false;
            numSelected--;
          }
        }
      }
  }
  if(flag == 0) Serial.println("RS_SL_AP");
}
int SCAN::numOfAPSelected() {
  return numSelected;
}
void SCAN::printSelected() {
  for (int i = 0; i < numOfNetwork && i < maxNumOfNetwork; i++) {
    if (selected[i] == true) {
      Serial.println(bssid._get(i).toString());
    }
  }
  delay(1);
}
void SCAN::parseEssid(){
  for(int i=0; i<numOfNetwork && i < maxNumOfNetwork; i++){
    essid[i].replace("/"," ");
  }
}



