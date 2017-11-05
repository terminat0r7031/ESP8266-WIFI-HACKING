#ifndef MACLIST_H
#define MACLIST_H
#include"MAC.h"
class MACLIST {
  private:
    MAC* mac;
    const int listLen;
    int currentId;
  public:
    MACLIST(int l = 64);
    ~MACLIST();
    void add(const MAC& m);
    MAC _get(int i);
    void _set(const MAC& m, int id);
    void clearAll();
};
#endif
