/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeTest1.h"

ModeTest1::ModeTest1(NixieArray *nxa):  ModeBase::ModeBase(nxa,
                                                           "ModeTest1",
                                                           ModeTest1::TICK_MS) {
}

void ModeTest1::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  Serial.println("ModeTest1::init>");

  this->_digit = 0;
  this->_prev_digit = 9;

  for (int i=0; i < NIXIE_NUM_N; i++) {
    for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
      if (d == this->_prev_digit) {
        this->_nxa->num[i].element[d].set_blightness(BLIGHTNESS_RESOLUTION);
      } else {
        this->_nxa->num[i].element[d].set_blightness(0);
      }
    } // for(d)
  } // for(i)

  for (int i=0; i < NIXIE_COLON_N; i++) {
    Serial.println("i=" + String(i));
    for (int d=0; d < NIXIE_COLON_DOT_N; d++) {
      Serial.println(" d=" + String(d));
      this->_nxa->colon[i].element[d].set_blightness(BLIGHTNESS_RESOLUTION);
    } // for (d)
    this->_nxa->colon[i].blink_start(start_ms, 500, NIXIE_COLON_DOT_UP);
  } // for (i)

  Serial.println("ModeTest1::init> done");
}

void ModeTest1::loop(unsigned long cur_ms, DateTime& now) {
  if ( ! this->tick(cur_ms) ) {
    return;
  }
  // Serial.println("ModeTest1::loop> digit=" + String(this->_digit));

  // -------------------------------------------------------------------------
  // num
  int num = 0;
  this->_nxa->num[num].element[this->_prev_digit].set_blightness(0);
  //this->_nxa->num[num].element[this->_digit].set_blightness(BLIGHTNESS_RESOLUTION);
  this->_nxa->num[num].element[this->_digit].set_blightness(1);
  
  num = 1;
  this->_nxa->num[num].element[this->_prev_digit].set_blightness(0);
  this->_nxa->num[num].fadein_start(cur_ms, this->FADE_TICK_MS,
                                    this->_digit);

  num = 2;
  this->_nxa->num[num].element[this->_digit].set_blightness(BLIGHTNESS_RESOLUTION);
  this->_nxa->num[num].fadeout_start(cur_ms, this->FADE_TICK_MS,
                                     this->_prev_digit);
  
  num = 3;
  this->_nxa->num[num].xfade_start(cur_ms, this->FADE_TICK_MS,
                                   this->_digit, this->_prev_digit);

  num = 4;
  this->_nxa->num[num].shuffle_start(cur_ms, this->SHUFFLE_TICK_MS,
                                     this->SHUFFLE_COUNT, this->_digit);

  num = 5;
  this->_nxa->num[num].end_effect();
  this->_nxa->num[num].element[this->_prev_digit].set_blightness(0);
  this->_nxa->num[num].element[this->_digit].set_blightness(BLIGHTNESS_RESOLUTION);
  this->_nxa->num[num].blink_start(cur_ms, 100, NIXIE_NUM_DIGIT_N);
  
  // -------------------------------------------------------------------------
  this->_prev_digit = this->_digit;
  this->_digit = (this->_digit + 1) % 10;
} // ModeTest1::loop()

void ModeTest1::btn_hdr(unsigned long cur_ms, Button *btn) {
  Serial.println("ModeTest1::btn_hdr(" + btn->get_name() + ")");
} // ModeTest1::btn_hdr()

// for emacs
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
