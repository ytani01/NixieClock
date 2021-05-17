/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef MODE_SET_CLOCK_H
#define MODE_SET_CLOCK_H
#include "ModeBase.h"

class ModeSetClock : public ModeBase {
 public:
  static const unsigned long MODE_NULL   = 0x00;

  static const unsigned long MODE_DATE   = 0x10;
  static const unsigned long MODE_YEAR   = 0x11;
  static const unsigned long MODE_MONTH  = 0x12;
  static const unsigned long MODE_DAY    = 0x13;

  static const unsigned long MODE_TIME   = 0x20;
  static const unsigned long MODE_HOUR   = 0x21;
  static const unsigned long MODE_MINUTE = 0x22;

  static const unsigned long TICK_MS = 10;        // ms
  static const unsigned long BLINK_TICK_MS = 500; // ms

  ModeSetClock(NixieArray *nxa);
  void init(unsigned long start_ms, DateTime& now, int init_val[NIXIE_NUM_N]);
  void loop(unsigned long cur_ms, DateTime& now);
  void btn_intr_hdr(unsigned long cur_ms, Button *btn);
  void btn_loop_hdr(unsigned long cur_ms, Button *btn);

  unsigned long change_mode(unsigned long mode);

 private:
  unsigned long _mode;
  
}; // class ModeSetClock
#endif // MODE_SET_CLOCK_H
