/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeBase.h"

<<<<<<< HEAD
ModeBase::ModeBase(NixieArray *nxa) {
  this->_nxa = nxa;

  this->_name = "ModeBase";
  this->_tick_ms = ModeBase::TICK_MS;

  String msg = "ModeBase(): ";
=======
ModeBase::ModeBase(NixieArray *nxa, String name, unsigned long tick_ms) {
  this->_nxa = nxa;
  this->_name = name;
  this->_tick_ms = tick_ms;

  String msg = "ModeBase::setup(): ";
>>>>>>> 839482fcb75592cc851d3000635618b63c0a7235
  msg += "_name = " + this->_name;
  msg += ", ";
  msg += "_tick_ms = " + this->_tick_ms;
  Serial.println(msg);
}

<<<<<<< HEAD
=======
boolean ModeBase::tick(unsigned long cur_ms) {
  if ( this->_tick_ms == 0 ) {
    Serial.println("!? _tick_ms=" + String(this->_tick_ms));
    return false;
  }
  this->_prev_tick = this->_tick;
  this->_tick = (cur_ms - this->_start_ms) / this->_tick_ms;
  if ( this->_tick == this->_prev_tick ) {
    return false;
  }
  return true;
} // ModeBase::tick()

>>>>>>> 839482fcb75592cc851d3000635618b63c0a7235
void ModeBase::init(unsigned long start_ms) {
  this->_start_ms = start_ms;
  (void)this->tick(start_ms);
  
  String msg = "ModeBase::init(): ";
  msg += "_start_ms = " + String(this->_start_ms);
  Serial.println(msg);
}

void ModeBase::loop(unsigned long cur_ms) {
  if ( ! this->tick(cur_ms) ) {
    return;
  }
  Serial.println("ModeBase::loop()");
}

void ModeBase::btn_intr(unsigned long cur_ms, Button *btn) {
  Serial.println("ModeBase::btn_intr()");
} // ModeBase::btn_intr()

boolean ModeBase::tick(unsigned long cur_ms) {
  if ( this->_tick_ms == 0 ) {
    Serial.println("!? _tick_ms=" + String(this->_tick_ms));
    return false;
  }
  this->_prev_tick = this->_tick;
  this->_tick = (cur_ms - this->_start_ms) / this->_tick_ms;
  if ( this->_tick == this->_prev_tick ) {
    return false;
  }
  return true;
} // ModeBase::tick()

// for emacs
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
