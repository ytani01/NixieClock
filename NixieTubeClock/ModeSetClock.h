/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef MODE_SET_CLOCK_H
#define MODE_SET_CLOCK_H
#include "ModeBase.h"

class ModeSetClock : public ModeBase {
 public:
  static const unsigned long MODE_NULL = 0x00;
  static const unsigned long MODE_DATE = 0x01;
  static const unsigned long MODE_TIME = 0x02;

  static const unsigned long TICK_MS = 10;        // ms
  static const unsigned long BLINK_TICK_MS = 500; // ms

  ModeSetClock(NixieArray *nxa);
  void init(unsigned long start_ms, int init_val[NIXIE_NUM_N]);

 private:
  
}; // class ModeSetClock
#endif // MODE_SET_CLOCK_H
