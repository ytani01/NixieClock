/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H
#include <Arduino.h>
#include "NixieArray.h"
#include "Button.h"

class ModeBase {
 public:
  static const unsigned long TICK_MS = 5000; // ms
  static const unsigned long EFFECT_TICK_MS = 200; // ms
  
  ModeBase(NixieArray *nxa, String name, unsigned long tick_ms);

  String name();
  boolean tick(unsigned long cur_ms);

  virtual void init(unsigned long start_ms, DateTime& now,
                    int init_val[NIXIE_NUM_N]); // モード変更時の初期化
  virtual void loop(unsigned long cur_ms, DateTime& now);  // loop()内での処理
  virtual void btn_intr_hdr(unsigned long cur_ms, Button *btn); // ボタン処理
  virtual void btn_loop_hdr(unsigned long cur_ms, Button *btn); // ボタン処理

 protected:
  String        _name;         // モード名
  NixieArray   *_nxa;          // ニキシー管アレイ
  unsigned long _start_ms;
  unsigned long _tick_ms;      // tick間隔(msec): loop()で処理する間隔
  unsigned long _tick;         // tickカウント
  unsigned long _prev_tick;

  unsigned _num[NIXIE_NUM_N];
};
#endif // MODE_BASE_H
