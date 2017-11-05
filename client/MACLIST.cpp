#include"MACLIST.h"
MACLIST::MACLIST(int l): listLen(l) {
  mac = new MAC [listLen];
  currentId = 0;
}
MACLIST::~MACLIST() {
  delete [] mac;
}
void MACLIST::add(const MAC& m) {
  mac[currentId] = m;
  currentId++;
}
MAC MACLIST::_get(int i) {
  return mac[i];
}
void MACLIST::_set(const MAC& m, int id) {
  mac[id] = m;
}

void MACLIST::clearAll() {
  for (int i = 0; i < currentId; i++) {
    mac[i] = MAC(0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
  }
  currentId = 0;
}


