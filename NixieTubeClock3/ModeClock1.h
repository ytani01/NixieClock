/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef MODE_CLOCK1_H
#define MODE_CLOCK1_H
#include "ModeBase.h"

class ModeClock1 : public ModeBase {
 public:
  static const unsigned long TICK_MS = 300;        // ms
  static const unsigned long FADE_TICK_MS = 70;    // ms
  static const unsigned long SHUFFLE_TICK_MS = 10; // ms
  static const unsigned long SHUFFLE_COUNT = 60;
  static const unsigned long DT_STR_LEN = 64;      // chars

  ModeClock1(NixieArray *nxa);
  void init(unsigned long start_ms);
  void loop(unsigned long cur_ms);
  void btn_intr(unsigned long cur_ms, Button *btn);

 private:
  RTC_DS3231 _rtc;
  int _num[NIXIE_NUM_N];
};
#endif // MODE_CLOCK1_H

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
