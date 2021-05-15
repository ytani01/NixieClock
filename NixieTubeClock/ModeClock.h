/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef MODE_CLOCK_H
#define MODE_CLOCK_H
#include "ModeBase.h"

/**
 *
 */
class ModeClock : public ModeBase {
 public:
  static const unsigned long MODE_NULL = 0x00;
  static const unsigned long MODE_HMS = 0x01;
  static const unsigned long MODE_DHM = 0x02;
  static const unsigned long MODE_YMD = 0x03;
    
  static const unsigned long TICK_MS = 10;         // ms
  static const unsigned long FADE_TICK_MS = 40;    // ms
  static const unsigned long SHUFFLE_TICK_MS = 10; // ms
  static const unsigned long SHUFFLE_COUNT = 60;
  static const unsigned long DT_STR_LEN = 64;      // chars

  unsigned long mode = MODE_HMS;
  unsigned long ch_mode_ms = 0;
  static const unsigned long CH_MODE_MS_LIMIT = 2000;

  ModeClock(NixieArray *nxa);
  void init(unsigned long start_ms);
  void loop(unsigned long cur_ms, DateTime& now);
  void change_mode(unsigned long mode);
  void btn_intr_hdr(unsigned long cur_ms, Button *btn);
  void btn_loop_hdr(unsigned long cur_ms, Button *btn);

 private:
  RTC_DS3231 _rtc;
  int _num[NIXIE_NUM_N];
};
#endif // MODE_CLOCK_H
