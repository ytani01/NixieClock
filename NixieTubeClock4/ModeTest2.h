/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef MODE_TEST2_H
#define MODE_TEST2_H
#include "ModeBase.h"

class ModeTest2 : public ModeBase {
 public:
  static const unsigned long TICK_MS = 1000; // ms
  static const unsigned long FADE_TICK_MS = 90; // ms
  static const unsigned long SHUFFLE_TICK_MS = 10; // ms
  static const unsigned long SHUFFLE_COUNT = 60;
  
  ModeTest2(NixieArray *nxa);
  void init(unsigned long start_ms);
  void loop(unsigned long cur_ms, DateTime& now);
  void btn_hdr(unsigned long cur_ms, Button *btn);

 private:
  int *_digit;
  int _cur;
};
#endif // MODE_TEST2_H

// Local Variables:
// Mode: c++-mode
// Coding: utf-8-unix
// End:
