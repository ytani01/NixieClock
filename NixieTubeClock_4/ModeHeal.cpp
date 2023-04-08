/**
 * Copyright 2023 (c) Yoichi Tanibayashi
 */
#include "ModeHeal.h"

/**
 *
 */
ModeHeal::ModeHeal(NixieArray *nxa)
  : ModeBase::ModeBase(nxa, "Heal", ModeHeal::TICK_MS) {
} // ModeHeal::ModeHeal()

/**
 *
 */
void ModeHeal::init(unsigned long start_ms, DateTime& now,
                    int init_val[NIXIE_NUM_N]) {
  ModeBase::init(start_ms, now, init_val);
  for (int i=0; i < NIXIE_COLON_N; i++) {
    NxColEl(i, NIXIE_COLON_DOT_DOWN).set_brightness(0);
  } // for(i)
  
  Nx->brightness = BRIGHTNESS_MAX;

  this->stat = STAT_DONE;
} // ModeHeal::init()

/**
 *
 */
stat_t ModeHeal::loop(unsigned long cur_ms, DateTime& now) {
  if ( ModeBase::loop(cur_ms, now) == STAT_SKIP ) {
    return STAT_SKIP;
  }
  return this->stat;
} // ModeHeal::loop()

/**
 *
 */
void ModeHeal::btn_loop_hdr(unsigned long cur_ms, Button *btn) {
  
} // ModeHeal::btn_loop_hdr()
