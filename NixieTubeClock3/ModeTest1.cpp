/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeTest1.h"

void ModeTest1::setup(int idx, NixieArray *nxa) {
  ModeBase::setup(idx, nxa);

  this->_name = "ModeTest1";

  Serial.print("TestMode1::setup()");
  Serial.print(": _name=" + this->_name);
  Serial.print(", _idx=" + String(this->_idx));
  Serial.println();

}

void ModeTest1::init() {
  ModeBase::init();

  // XXX
}

void ModeTest1::loop(unsigned long cur_ms) {
  ModeBase::loop(cur_ms);

  static unsigned long interval = 2000; // ms
  static unsigned long num = 0;

  static unsigned long prev_n = 0;
  unsigned long cur_n = cur_ms / interval;

  if ( cur_n != prev_n ) {
    // XXX
    
    prev_n = cur_n;
    num = (num + 1) % 10;
  }
} // ModeTest1::loop()

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
