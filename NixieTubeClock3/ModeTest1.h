/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef MODE_TEST1_H
#define MODE_TEST1_H
#include "ModeBase.h"

class ModeTest1 : public ModeBase {
 public:
  static const unsigned long TICK_MS = 2000; // ms
  static const unsigned long EFFECT_TICK_MS = 150; // ms
  
  void setup(int idx, NixieArray *nxa);
  void init(unsigned long start_ms);
  void loop(unsigned long cur_ms);
  void btn_intr(unsigned long cur_ms, Button *btn);

 private:
  int _digit;
  int _prev_digit;
};
#endif // MODE_TEST1_H

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
