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
  
<<<<<<< HEAD
  ModeBase(NixieArray *nxa);
=======
  ModeBase(NixieArray *nxa, String name, unsigned long tick_ms);

  boolean tick(unsigned long cur_ms);

>>>>>>> 839482fcb75592cc851d3000635618b63c0a7235
  virtual void init(unsigned long start_ms); // モード変更時の初期化:loop()内
  virtual void loop(unsigned long cur_ms);   // loop()内での処理
  virtual void btn_intr(unsigned long cur_ms, Button *btn); // ボタン処理
  boolean tick(unsigned long cur_ms);

 protected:
  String        _name;
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
