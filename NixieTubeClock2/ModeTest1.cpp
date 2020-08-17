/*
 *
 */
#include "ModeTest1.h"

void ModeTest1::setup(int idx, NixieTubeArray *nxa, CmdQueue *cmd_q) {
  ModeBase::setup(idx, nxa, cmd_q);
  this->_name = "ModeTest1";

  Serial.print("TestMode1.setup()");
  Serial.print(": _name=" + this->_name);
  Serial.print(", _idx=" + String(this->_idx));
  Serial.println();

  
}

void ModeTest1::loop(unsigned long cur_ms) {
  ModeBase::loop(cur_ms);
  if (cur_ms % 1000) {
    Serial.println("ModeTest1::loop(" + String(cur_ms) + ")");

    for (int d=0; d < NIXIE_TUBE_DIGIT_N; d++) {
      this->_cmd_q->set_digit(0, d, 0);
    } // for(d)
    this->_cmd_q->set_digit(0, cur_ms % 1000, BLIGHTNESS_MAX);
  }
}
