/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef MODE_SCORE1_H
#define MODE_SCORE1_H
#include "ModeBase.h"

class ModeScore1 : public ModeBase {
 public:
  static const unsigned long TICK_MS = 100; // ms
  static const unsigned long BLINK_TICK_MS = 200; // ms

  static const mode_t MODE_NONE   = 0x00;
  static const mode_t MODE_MODIFY = 0x01;

  static const int NUM_N = 3;
  
  ModeScore1(NixieArray *nxa);
  void init(unsigned long start_ms, DateTime& now,
            int init_val[NIXIE_NUM_N]);
  stat_t loop(unsigned long cur_ms, DateTime& now);
  void btn_intr_hdr(unsigned long cur_ms, Button *btn);
  void btn_loop_hdr(unsigned long cur_ms, Button *btn);

  void get_disp_str(char* disp_str);

 private:
  mode_t _mode;
  int _score[NUM_N];
  int _cur;
};
#endif // MODE_SCORE1_H
