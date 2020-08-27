/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeBase.h"

unsigned long ModeBase::calc_tick(unsigned long cur_ms) {
  if ( this->_tick_ms == 0 ) {
    return 0;
  }
  return (cur_ms - this->_start_ms) / this->_tick_ms;
} // ModeBase::calc_tick()

void ModeBase::setup(int idx, NixieArray *nxa) {
  this->_idx = idx;
  this->_nxa = nxa;

  this->_name = "ModeBase";
  this->_tick_ms = ModeBase::TICK_MS;

  String msg = "ModeBase::setup(): ";
  msg += "_idx = " + String(this->_idx);
  msg += ", ";
  msg += "_name = " + this->_name;
  msg += ", ";
  msg += "_tick_ms = " + this->_tick_ms;
  Serial.println(msg);
}

void ModeBase::init(unsigned long start_ms) {
  this->_start_ms = start_ms;
  this->_tick = this->calc_tick(start_ms);
  this->_prev_tick = this->_tick;
  
  String msg = "ModeBase::init(): ";
  msg += "_start_ms = " + String(this->_start_ms);
  msg += ", ";
  msg += "_tick_ms = " + String(this->_tick_ms);
  msg += ", ";
  msg += "_tick = " + String(this->_tick);
  msg += ", ";
  msg += "_prev_tick = " + String(this->_prev_tick);
  Serial.println(msg);
}

void ModeBase::loop(unsigned long cur_ms) {
  this->_prev_tick = this->_tick;
  this->_tick = calc_tick(cur_ms);
  if ( this->_tick == this->_prev_tick ) {
    return;
  }
  Serial.println("ModeBase::loop()");
}

void ModeBase::btn_intr(unsigned long cur_ms, Button *btn) {
  Serial.println("ModeBase::btn_intr()");
} // ModeBase::btn_intr()
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
