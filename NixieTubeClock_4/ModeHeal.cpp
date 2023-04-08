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
void ModeHeal::init(unsigned long start_ms,
                    DateTime& now,
                    int init_val[NIXIE_NUM_N]) {

  ModeBase::init(start_ms, now, init_val);

  this->_startMs = millis();
  
  this->stat = STAT_NONE;
} // ModeHeal::init()

/**
 *
 */
stat_t ModeHeal::loop(unsigned long cur_ms, DateTime& now) {

  if ( ModeBase::loop(cur_ms, now) == STAT_SKIP ) {
    return STAT_SKIP;
  }

  if ( millis() - this->_startMs >= HEALING_TIME_MS ) {
    log_i("cur_ms=%u, startMs=%u: %u, %u",
          cur_ms, this->_startMs, cur_ms - this->_startMs, HEALING_TIME_MS);
          
    return STAT_DONE;
  }
  
  for (int i=0; i < NIXIE_NUM_N; i++) {
    for ( int e=0; e <=9; e++) {
      NxNumEl(i, e).set_brightness(1);
    }
  }
  for (int i=0; i < NIXIE_COLON_N; i++) {
    NxColEl(i, NIXIE_COLON_DOT_DOWN).set_brightness(0);
  } // for(i)

  if ( this->_targetNum < 0 ) {
    this->_startMs = millis();
    return this->stat;
  }

  for (int i=0; i < NIXIE_NUM_N; i++) {
    for ( int e=0; e <=9; e++) {
      if ( i == this->_targetNum && e == this->_targetElement ) {
        NxNumEl(i, e).set_brightness(BRIGHTNESS_HEAL);
      } else {
        NxNumEl(i, e).set_brightness(0);
      }
    }
  }
  
  return this->stat;
} // ModeHeal::loop()

/**
 *
 */
void ModeHeal::btn_loop_hdr(unsigned long cur_ms, Button *btn) {
  String btn_name = btn->get_name();

  if ( btn_name == "BTN2" ) {
    this->_targetElement += btn->get_click_count();

    if ( btn->is_repeated() ) {
      this->_targetElement++;
    }
    
    this->_targetElement %= 10;

    this->_startMs = millis();
    return;
  } // BTN2

  if ( btn_name == "BTN1" ) {
    this->_targetNum += btn->get_click_count();

    if ( btn->is_repeated() ) {
      this->_targetNum++;
    }

    this->_targetNum %= NIXIE_NUM_N;

    this->_startMs = millis();
    return;
  } // BTN1
} // ModeHeal::btn_loop_hdr()
