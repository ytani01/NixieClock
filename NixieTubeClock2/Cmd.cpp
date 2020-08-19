/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "Cmd.h"
//============================================================================
void Cmd::init(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]) {
  this->_nxa = nxa;
  this->_cmd = cmd;
  for (int p; p < CMD_PARAM_N; p++) {
    this->_param[p] = param[p];
  }
  Serial.println("cmd=" + this->toString());
}

void Cmd::start() {
  this->_working = true;
}
void Cmd::end() {
  this->_working = false;
}
boolean Cmd::is_null() {
  return this->_cmd == CMD_NULL;
}
boolean Cmd::is_working() {
  return this->_working;
}

void Cmd::loop(unsigned long cur_ms) {
  this->end();
}

String Cmd::toString() {
  String str = "";
  str += String(this->_cmd, HEX);
  return str;
}
//============================================================================
void CmdSleep::start() {
  Cmd::start();

  this->_start_ms = millis();
  this->_interval_ms = (unsigned long)this->_param[0];
}

void CmdSleep::loop(unsigned long cur_ms) {
  if ( cur_ms - this->_start_ms >= this->_interval_ms ) {
    this->end();
  }
  delay(1);
}
//============================================================================
void CmdSetDigit::start() {
  uint8_t nixie_i = (uint8_t)this->_param[0];
  uint8_t digit_i = (uint8_t)this->_param[1];
  uint8_t blightness = (uint8_t)this->_param[2];

  this->_nxa->set_num_blightness(nixie_i, digit_i, blightness);

  this->end();
}
//============================================================================
