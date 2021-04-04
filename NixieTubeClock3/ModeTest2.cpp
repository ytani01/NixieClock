/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeTest2.h"

ModeTest2::ModeTest2(NixieArray *nxa)
: ModeBase::ModeBase(nxa, "ModeTest2", ModeTest2::TICK_MS) {

  this->_digit_n = NIXIE_NUM_N;
  this->_digit = new int[this->_digit_n];
  this->_cur = 0;

  String msg = "ModeTest2::setup(): ";
  msg += "_name = " + this->_name;
  msg += ", ";
  msg += "_tick_ms = " + this->_tick_ms;
  Serial.println(msg);
}

void ModeTest2::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  Serial.println("ModeTest2::init>");

  for (int i=0; i < this->_digit_n; i++) {
    this->_digit[i] = i;
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      if ( this->_digit[i] == e ) {
        // this->_nxa->num[i].element[e].set_blightness(BLIGHTNESS_MAX);
        this->_nxa->num[i].randomOnOff_start(start_ms, 50, e);
      } else {
        this->_nxa->num[i].element[e].set_blightness(0);
      }
    } // for(e)
  } // for(i)
  this->_nxa->num[this->_cur].blink_start(start_ms, 200, NIXIE_NUM_DIGIT_N);
}

void ModeTest2::loop(unsigned long cur_ms) {
  if ( ! this->tick(cur_ms) ) {
    return;
  }

  if ( cur_ms - this->_start_ms > 1000 ) {
    for (int i=0; i < _digit_n; i++) {
      this->_nxa->num[i].end_effect();
    }
  }
  
  String msg = "_digit=[ ";
  for (int i=0; i < this->_digit_n; i++) {
    if (i == this->_cur) {
      msg += "<" + String(this->_digit[i]) + "> ";
    } else {
      msg += String(this->_digit[i]) + " ";
    }
  } // for(i)
  msg += "]";
  Serial.println(msg);
} // ModeTest2::loop()

void ModeTest2::btn_intr(unsigned long cur_ms, Button *btn) {
  Serial.println("ModeTest2::btn_intr(" + btn->get_name() + ")");

  if ( btn->get_name() == "BTN1" ) {
    if ( btn->get_click_count() > 0 || btn->is_repeated() ) {
      this->_nxa->num[this->_cur].end_effect();
      this->_cur = (this->_cur + 1) % NIXIE_NUM_N;
      this->_nxa->num[this->_cur].blink_start(cur_ms, 200, NIXIE_NUM_DIGIT_N);
    }
  }
} // ModeTest2::btn_intr()

// for emacs
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
