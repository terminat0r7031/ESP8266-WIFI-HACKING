#include<SerialCommand.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include<FS.h>
#include"data.h"
#define bufSize 6000

int partID = 0;
int numPart = 0;
String json = "";
bool _status = 0;       // status == 0 -> stopped | status == 1 -> running
bool _monitorState = 0;
int bufc = 0;
char data_websiteBuffer[bufSize];

ESP8266WebServer server(80);
SerialCommand sCmd;

const char* essid = "abcdabcd";
const char* password = "khongbietdau";

//----KHAI BAO PROTOTYPE----------//
void startWifi();
void requestStartScanAP();
void responseStartScanAP();
void requestSendScanAPResults();
void sendScanAPResults();
void requestSelectAP();
void responeSelectAP();

void requestStartScanStation();
void responseStartScanStation();
void requestSendScanStationResults();
void responseSendScanStationResults();
void sendScanStationResults();
void requestSelectStation();
void responseSelectStation();
void requestBeaconAttack();
void responseBeaconAttack();
void requestDeDisAttack();
void responseDeDisAttack();
void requestStartMonitorEnv();
void responseStartMonitorEnv1();
void responseStartMonitorEnv2();
void sendMonitorResults();
void checkStatus();
void loadIndexHTML();
void loadScanApHTML();
void loadScriptJS();
void loadStyleCSS();

String charToString(const char* s);
void sendBuffer();
void sendToBuffer(String str);
void sendHeader(int code, String type, size_t _size);
void sendFile(int code, String type, const char* adr, size_t len);


//---- KHỞI TẠO ACCESS POINT ----//
void startWifi() {
  WiFi.softAP(essid, password);
}

//---- 1. QUÉT MẠNG WIFI ----//

//---------------------------------
void requestStartScanAP() {
  // Gửi lệnh cho mạch CLIENT
  _status = 1;            // running
  Serial.println("RQ_S_S_AP");
  server.send(200, "text/json", "true");
}
void responseStartScanAP() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;            // stopped
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---------------------------------
void requestSendScanAPResults() {
  _status = 1;
  Serial.println("RQ_S_AP_R");
  server.send(200, "text/json", "true");
}
void responseSendScanAPResults() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu = sCmd.next();
  if (argu != NULL) {
    numPart = atoi(argu);
    argu = sCmd.next();
    if (argu != NULL) {
      json += charToString(argu);
      partID++;
      if (partID == numPart) {
        numPart = 0;
        partID = 0;
        _status = 0;
      }
    }
  }
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}
void sendScanAPResults() {
  size_t _size = (size_t) json.length();
  sendToBuffer(json);
  sendHeader(200, "text/json", _size);
  sendBuffer();
  json = "";
}

//---------------------------------
void requestSelectAP() {
  if (server.hasArg("id")) {
    _status = 1;
    String cmd = "RQ_SL_AP ";
    cmd += server.arg("id").toInt();
    Serial.println(cmd);
    server.send(200, "text/json", "true");
  }
}
void responeSelectAP() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 2. QUÉT STATION ----//

//---------------------------------
void requestStartScanStation() {
  if (server.hasArg("time")) {
    _status = 1;
    String cmd = "RQ_S_S_S ";
    cmd += server.arg("time").toInt();
    Serial.println(cmd);
    server.send(200, "text/json", "true");
  }
}
void responseStartScanStation() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---------------------------------
void requestSendScanStationResults() {
  _status = 1;
  Serial.println("RQ_S_ST_R");
  server.send(200, "text/json", "true");
}
void responseSendScanStationResults() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu = sCmd.next();
  if (argu != NULL) {
    numPart = atoi(argu);
    argu = sCmd.next();
    if (argu != NULL) {
      json += charToString(argu);
      partID++;
      if (partID == numPart) {
        numPart = 0;
        partID = 0;
        _status = 0;
      }
    }
  }
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}
void sendScanStationResults() {
  size_t _size = (size_t) json.length();
  sendToBuffer(json);
  sendHeader(200, "text/json", _size);
  sendBuffer();
  json = "";
}
//---------------------------------
void requestSelectStation() {
  if (server.hasArg("id")) {
    _status = 1;          // running
    String cmd = "RQ_SL_ST ";
    cmd += server.arg("id").toInt();
    Serial.println(cmd);
    server.send(200, "text/json", "true");
  }
}
void responseSelectStation() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;            // stopped
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 3. TẤN CÔNG BEACON FLOOD ----//

void requestBeaconAttack() {
  if(server.hasArg("random") && server.hasArg("time") && server.hasArg("ssid")){
    int rd = server.arg("random").toInt();
    if(rd==1){
      _status = 1;
      String cmd = "RQ_BC_AT ";
      cmd += rd;
      cmd += " ";
      cmd += server.arg("time").toInt();
      Serial.println(cmd);
      server.send(200,"text/json","true");
    }
    else{
      _status = 1;
      String cmd = "RQ_BC_AT ";
      cmd += rd;
      cmd += " ";
      cmd += server.arg("time").toInt();
      cmd += " ";
      cmd += server.arg("ssid");
      Serial.println(cmd);
      server.send(200,"text/json","true");
    }
  }
//  if (server.hasArg("ssid") && server.hasArg("time")) {
//    _status = 1;
//    String cmd = "RQ_BC_AT ";
//    cmd += server.arg("ssid");
//    cmd += " ";
//    cmd += server.arg("time").toInt();
//    Serial.println(cmd);
//    server.send(200, "text/json", "true");
//  }
}

void responseBeaconAttack() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 4. TẤN CÔNG DEAUTHENTICATION/DISASSOCIATION ----//
void requestDeDisAttack() {
  if (server.hasArg("time") && server.hasArg("type") && server.hasArg("all")) {
    _status = 1;
    int type = server.arg("type").toInt();
    if (type == 0) { //deauthentication
      String cmd = "RQ_DE_AT ";
      cmd += server.arg("time").toInt();
      cmd += " ";
      cmd += server.arg("all").toInt();
      Serial.println(cmd);
      server.send(200, "text/json", "true");
    }
    else {
      String cmd = "RQ_DIS_AT ";
      cmd += server.arg("time").toInt();
      cmd += " ";
      cmd += server.arg("all").toInt();
      Serial.println(cmd);
      server.send(200, "text/json", "true");
    }
  }
}
void responseDeDisAttack() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 5. GIÁM SÁT 3 FRAME TRÊN MÔI TRƯỜNG KHÔNG DÂY ----//
void requestStartMonitorEnv() {
  if (server.hasArg("time") && server.hasArg("channel")) {
    _status = 1;
    String cmd = "RQ_MON ";
    cmd += server.arg("time").toInt();
    cmd += " ";
    cmd += server.arg("channel").toInt();
    Serial.println(cmd);
    server.send(200, "text/json", "true");
  }
}
void responseStartMonitorEnv1() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu = sCmd.next();
  if (argu != NULL) {
    json = charToString(argu);
  }
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}
void responseStartMonitorEnv2() {
  digitalWrite(BUILTIN_LED, LOW);
  _status = 0;
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}
void sendMonitorResults() {
  size_t _size = (size_t) json.length();
  sendToBuffer(json);
  sendHeader(200, "text/json", _size);
  sendBuffer();
}

//---- CHECK STATUS ----//
void checkStatus() {
  if (_status) {
    server.send(200, "text/json", "true");
  }
  else {
    server.send(200, "text/json", "false");
  }
}

//------LOAD HTML, CSS, JAVASCRIPT -------//
void loadIndexHTML() {
  sendFile(200, "text/html", data_indexHTML, sizeof(data_indexHTML));
}
void loadScanApHTML(){
  sendFile(200, "text/html", data_scanApHTML, sizeof(data_scanApHTML));
}
void loadMonitorHTML(){
  sendFile(200, "text/html", data_monitorHTML, sizeof(data_monitorHTML));
}
void loadScriptJS() {
  sendFile(200, "text/javascript", data_scriptJS, sizeof(data_scriptJS));
}

void loadStyleCSS() {
  sendFile(200, "text/css;charset=UTF-8", data_styleCSS, sizeof(data_styleCSS));
}


String charToString(const char* s) {
  String str = "";
  int i = 0;
  while (s[i] != NULL) {
    str += s[i];
    i++;
  }
  return str;
}
void sendBuffer() {
  if (bufc > 0) {
    server.sendContent_P(data_websiteBuffer, bufc);
    bufc = 0;
  }
}

void sendToBuffer(String str) {
  size_t len = str.length();
  if (bufc + len > bufSize) {
    server.sendContent_P(data_websiteBuffer, bufc);
    bufc = 0;
  }
  memcpy(data_websiteBuffer + bufc, str.c_str(), len);
  bufc += len;
}

void sendHeader(int code, String type, size_t _size) {
  server.sendHeader("Content-Length", (String) _size);
  server.send(code, type, "");
}
void sendFile(int code, String type, const char* adr, size_t len) {
  sendHeader(code, type, len);
  server.sendContent_P(adr, len);
  sendBuffer();
}
void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  startWifi();
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);

  //---- Các lệnh SerialCommand nhận được từ mạch CLIENT ----//
  sCmd.addCommand("RS_S_S_AP", responseStartScanAP);
  sCmd.addCommand("RS_S_AP_R", responseSendScanAPResults);
  sCmd.addCommand("RS_SL_AP", responeSelectAP);
  sCmd.addCommand("RS_S_S_S", responseStartScanStation);
  sCmd.addCommand("RS_S_ST_R", responseSendScanStationResults);
  sCmd.addCommand("RS_SL_ST", responseSelectStation);
  sCmd.addCommand("RS_BC_AT", responseBeaconAttack);
  sCmd.addCommand("RS_DE_DIS_AT", responseDeDisAttack);
  sCmd.addCommand("RS1_MON", responseStartMonitorEnv1);
  sCmd.addCommand("RS2_MON", responseStartMonitorEnv2);

  //---- JSON ----//
  server.on("/startScanAP.json", requestStartScanAP);
  server.on("/requestSendScanAPResults.json", requestSendScanAPResults);
  server.on("/sendScanAPResults.json", sendScanAPResults);
  server.on("/selectAP.json", requestSelectAP);
  server.on("/startScanStation.json", requestStartScanStation);
  server.on("/requestSendScanStationResults.json", requestSendScanStationResults);
  server.on("/sendScanStationResults.json", sendScanStationResults);
  server.on("/selectStation.json", requestSelectStation);
  server.on("/beaconAttack.json", requestBeaconAttack);
  server.on("/deDisAttack.json", requestDeDisAttack);
  server.on("/startMonitorEnv.json", requestStartMonitorEnv);
  server.on("/sendMonitorResults.json", sendMonitorResults);
  server.on("/checkStatus.json", checkStatus);

  server.on("/", loadIndexHTML);
  server.on("/index.html", loadIndexHTML);
  server.on("/scan.html", loadScanApHTML);
  server.on("/monitor.html", loadMonitorHTML);
  server.on("/js/script.js", loadScriptJS);
  server.on("/css/style.css", loadStyleCSS);
  server.begin();
}
void loop() {
  server.handleClient();
  sCmd.readSerial();
}


