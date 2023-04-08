/**
 * Copyright (c) 2023 Yoichi Tanibayashi
 */
#ifndef MODE_HEAL_H
#define MODE_HEAL_H
#include "ModeBase.h"

class ModeHeal : public ModeBase {
 public:
  //static const unsigned long HEALING_TIME_MS = 60 * 3 * 1000; // 3 minutes
  static const unsigned long HEALING_TIME_MS = 10 * 1000; // 10 seconds

  //static const uint8_t BRIGHTNESS_HEAL = BRIGHTNESS_RESOLUTION;
  static const uint8_t BRIGHTNESS_HEAL = 4;

  static const mode_t TICK_MS = 10;        // ms
  static const mode_t BLINK_TICK_MS = 200; // ms

  ModeHeal(NixieArray *nxa);
  void init(unsigned long start_ms, DateTime& now, int init_val[NIXIE_NUM_N]);
  stat_t loop(unsigned long cur_ms, DateTime& now);
  void btn_intr_hdr(unsigned long cur_ms, Button *btn) {};
  void btn_loop_hdr(unsigned long cur_ms, Button *btn);

 private:
  int _targetNum = -1;
  int _targetElement = 0;
  unsigned long _startMs = 0;
  
}; // class ModeHeal
#endif // MODE_HEAL_H
