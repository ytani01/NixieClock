/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeTest1.h"

void ModeTest1::setup(int idx, NixieArray *nxa) {
  ModeBase::setup(idx, nxa);

  this->_name = "ModeTest1";
  this->_tick_ms = ModeTest1::TICK_MS;

  String msg = "ModeTest1::setup(): ";
  msg += "_idx = " + String(this->_idx);
  msg += ", ";
  msg += "_name = " + this->_name;
  msg += ", ";
  msg += "_tick_ms = " + this->_tick_ms;
  Serial.println(msg);
}

void ModeTest1::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  this->_digit = 0;
  this->_prev_digit = 9;

  for (int num=0; num < NIXIE_NUM_N; num++) {
    for (int digit=0; digit < NIXIE_NUM_DIGIT_N; digit++) {
      if (digit == this->_prev_digit) {
        this->_nxa->num[num].element[digit].set_blightness(BLIGHTNESS_MAX);
      } else {
        this->_nxa->num[num].element[digit].set_blightness(0);
      }
    } // for(digit)
  } // for(num)

  for (int colon=0; colon < NIXIE_COLON_N; colon++) {
    for (int dot=0; dot < NIXIE_COLON_DOT_N; dot++) {
      this->_nxa->colon[colon].element[dot].set_blightness(BLIGHTNESS_MAX);
    } // for (dot)
  } // for (colon)
}

void ModeTest1::loop(unsigned long cur_ms) {
  ModeBase::loop(cur_ms);
  if ( this->_tick == this->_prev_tick ) {
    return;
  }
  // -------------------------------------------------------------------------
  // num
  int num=0;
  this->_nxa->num[num].element[this->_prev_digit].set_blightness(0);
  this->_nxa->num[num].element[this->_digit].set_blightness(BLIGHTNESS_MAX);
  
  num=1;
  this->_nxa->num[num].fadeout_start(cur_ms, this->EFFECT_TICK_MS,
                                     this->_prev_digit);
  this->_nxa->num[num].fadein_start(cur_ms, this->EFFECT_TICK_MS,
                                    this->_digit);
  // -------------------------------------------------------------------------
  // colon
  if (num % 2 == 0) {
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_UP].
      set_blightness(BLIGHTNESS_MAX);
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_DOWN].
      set_blightness(BLIGHTNESS_MAX);
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_UP].
      set_blightness(0);
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_DOWN].
      set_blightness(0);
  } else {
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_UP].
      set_blightness(0);
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_DOWN].
      set_blightness(0);
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_UP].
      set_blightness(BLIGHTNESS_MAX);
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_DOWN].
      set_blightness(BLIGHTNESS_MAX);
  }
  // -------------------------------------------------------------------------
  this->_prev_digit = this->_digit;
  this->_digit = (this->_digit + 1) % 10;
} // ModeTest1::loop()

void ModeTest1::btn_intr(unsigned long cur_ms, Button *btn) {
  ModeBase::btn_intr(cur_ms, btn);

  // XXX
} // ModeTest1::btn_intr()
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
