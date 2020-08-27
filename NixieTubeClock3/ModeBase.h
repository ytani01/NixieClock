/*
 *
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H
#include <Arduino.h>
#include "Nixie.h"
#include "Button.h"

class ModeBase {
 public:
  static const unsigned long TICK_MS = 5000; // ms
  static const unsigned long EFFECT_TICK_MS = 200; // ms
  
  unsigned long calc_tick(unsigned long cur_ms);

  virtual void setup(int idx, NixieArray *nxa); // setup()内での初期化
  virtual void init(unsigned long start_ms); // モード変更時の初期化:loop()内
  virtual void loop(unsigned long cur_ms); // loop()内での処理
  virtual void btn_intr(unsigned long cur_ms, Button *btn); // ボタン処理

 protected:
  String        _name;
  int           _idx;
  NixieArray   *_nxa;
  unsigned long _start_ms;
  unsigned long _tick_ms;
  unsigned long _tick;
  unsigned long _prev_tick;
};
#endif // MODE_BASE_H

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
