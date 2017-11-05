#include"MAC.h"
MAC::MAC(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t fifth, uint8_t sixth) {
  address[0] = first;
  address[1] = second;
  address[2] = third;
  address[3] = fourth;
  address[4] = fifth;
  address[5] = sixth;
}
MAC::MAC(uint8_t addr[6]) {
  for (int i = 0; i < 6; i++) {
    address[i] = addr[i];
  }
}
MAC::MAC(const MAC& m) {
  for (int i = 0; i < 6; i++) {
    address[i] = m.address[i];
  }
}
String MAC::toString() {
  String value = "";
  for (int i = 0; i < 6; i++) {
    if (address[i] < 0x10)
      value += "0";
    value += String(address[i], HEX);
    if (i < 5) value += ":";
  }
  return value;
}
void MAC::set(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t fifth, uint8_t sixth) {
  address[0] = first;
  address[1] = second;
  address[2] = third;
  address[3] = fourth;
  address[4] = fifth;
  address[5] = sixth;
}
void MAC::_print() {
  Serial.print(this->toString());
}
const MAC& MAC::operator=(const MAC& m) {
  if (this != &m) {
    for (int i = 0; i < 6; i++) {
      address[i] = m.address[i];
    }
  }
  return *this;
}
boolean MAC::operator==(const MAC& m) {
  for (int i = 0; i < 6; i++) {
    if (address[i] != m.address[i])
      return false;
  }
  return true;
}
boolean MAC::operator!=(const MAC& m) {
  if ((*this) == m)
    return false;
  return true;
}
boolean MAC::compare3Byte(const MAC& m) {
  for (int i = 0; i < 3; i++) {
    if (address[i] != m.address[i])
      return false;
  }
  return true;
}
MAC MAC:: _random() {
  for (int i = 0; i < 6; i++) {
    address[i] = random(0, 256);
  }
  return *this;
}
uint8_t MAC::_get(int i) {
  return address[i];
}

