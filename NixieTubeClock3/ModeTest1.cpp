/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeTest1.h"

void ModeTest1::setup(int idx, NixieArray *nxa) {
  ModeBase::setup(idx, nxa);

  this->_name = "ModeTest1";
  this->_digit = 0;

  String msg="TestMode1::setup()> ";
  msg += "_idx=" + String(this->_idx);
  msg += ", ";
  msg += "_name=" + this->_name;
  msg += ", ";
  msg += "_digit=" + String(this->_digit);
  Serial.println(msg);
}

void ModeTest1::init() {
  ModeBase::init();

  for (int num=0; num < NIXIE_NUM_N; num++) {
    for (int digit=0; digit < NIXIE_NUM_DIGIT_N; digit++) {
      if (digit == this->_digit) {
        this->_nxa->num[num].element[digit].set_blightness_max();
      } else {
        this->_nxa->num[num].element[digit].set_blightness_zero();
      }
    } // for(digit)
  } // for(num)

  for (int colon=0; colon < NIXIE_COLON_N; colon++) {
    for (int dot=0; dot < NIXIE_COLON_DOT_N; dot++) {
      this->_nxa->colon[colon].element[dot].set_blightness_max();
    } // for (dot)
  } // for (colon)
}

void ModeTest1::loop(unsigned long cur_ms) {
  ModeBase::loop(cur_ms);

  static const unsigned long interval = 2000; // ms
  static unsigned int prev_disp_digit = 9;
  static unsigned int disp_digit = 0;

  static unsigned long prev_n = 0;
  unsigned long cur_n = cur_ms / interval;

  if ( cur_n == prev_n ) {
    return;
  }

  // num ---------------------------------------------------------------------
  int num=0;
  this->_nxa->num[num].element[prev_disp_digit].set_blightness_zero();
  this->_nxa->num[num].element[disp_digit].set_blightness_max();
  
  num=1;
  this->_nxa->num[num].start_fade_out(cur_ms, prev_disp_digit, 200);
  this->_nxa->num[num].start_fade_in(cur_ms, disp_digit, 200);
  
  // colon -------------------------------------------------------------------
  if (num % 2 == 0) {
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_UP].
      set_blightness_max();
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_DOWN].
      set_blightness_max();
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_UP].
      set_blightness_zero();
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_DOWN].
      set_blightness_zero();
  } else {
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_UP].
      set_blightness_zero();
    this->_nxa->colon[NIXIE_COLON_L].element[NIXIE_COLON_DOT_DOWN].
      set_blightness_zero();
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_UP].
      set_blightness_max();
    this->_nxa->colon[NIXIE_COLON_R].element[NIXIE_COLON_DOT_DOWN].
      set_blightness_max();
  }

  prev_n = cur_n;
  prev_disp_digit = disp_digit;
  disp_digit = (disp_digit + 1) % 10;
} // ModeTest1::loop()

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
