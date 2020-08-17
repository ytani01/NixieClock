/*
 *
 */
#include "ModeBase.h"

void ModeBase::setup(int idx, NixieTubeArray *nxa, CmdQueue *cmd_q) {
  this->_idx = idx;
  this->_nxa = nxa;
  this->_cmd_q = cmd_q;
  this->_name = "ModeBase";

  Serial.print("ModeBase::setup()");
  Serial.print(": _name=" + this->_name);
  Serial.print(", _idx=" + String(this->_idx));
  Serial.println();
}

void ModeBase::loop(unsigned long cur_ms) {
  if (cur_ms % 5000) {
    Serial.println("ModeBase::loop(" + String(cur_ms) + ")");
  }
}
