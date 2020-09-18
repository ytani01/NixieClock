/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeTest2.h"

ModeTest2::ModeTest2(NixieArray *nxa)
: ModeBase::ModeBase(nxa, "ModeTest2", ModeTest2::TICK_MS) {

  this->_digit_n = NIXIE_NUM_N;
  this->_digit = new int[this->_digit_n];

  String msg = "ModeTest2::setup(): ";
  msg += "_name = " + this->_name;
  msg += ", ";
  msg += "_tick_ms = " + this->_tick_ms;
  Serial.println(msg);
}

void ModeTest2::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  for (int i=0; i < this->_digit_n; i++) {
    this->_digit[i] = 0;
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      if ( this->_digit[i] == e ) {
        this->_nxa->num[i].element[e].set_blightness(BLIGHTNESS_MAX);
      } else {
        this->_nxa->num[i].element[e].set_blightness(0);
      }
    } // for(e)
  } // for(i)
}

void ModeTest2::loop(unsigned long cur_ms) {
  if ( ! this->tick(cur_ms) ) {
    return;
  }
  String msg = "_digit=[ ";
  for (int i=0; i < this->_digit_n; i++) {
    msg += String(this->_digit[i]) + " ";
  } // for(i)
  msg += "]";
  Serial.println(msg);
} // ModeTest2::loop()

void ModeTest2::btn_intr(unsigned long cur_ms, Button *btn) {
  Serial.println("ModeTest2::btn_intr()");
} // ModeTest2::btn_intr()

// for emacs
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
