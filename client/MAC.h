#ifndef MAC_H
#define MAC_H
#include<Arduino.h>
class MAC {
  public:
    MAC(uint8_t first = 0, uint8_t second = 0, uint8_t third = 0, uint8_t fourth = 0, uint8_t fifth = 0, uint8_t sixth = 0);
    MAC(uint8_t addr[6]);
    MAC(const MAC& m);
    String toString();
    void set(uint8_t first, uint8_t second, uint8_t third, uint8_t fourth, uint8_t fifth, uint8_t sixth);
    void _print();
    const MAC& operator = (const MAC& m);
    boolean operator == (const MAC& m);
    boolean operator != (const MAC& m);
    uint8_t _get(int i);
    MAC _random();
    boolean compare3Byte(const MAC &m);
  private:
    uint8_t address[6];
};
#endif
