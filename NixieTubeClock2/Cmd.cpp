/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include <Arduino.h>
#include "Cmd.h"
//============================================================================
Cmd::Cmd(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]) {
  this->_nxa = nxa;
  this->_cmd = cmd;
  for (int p; p < CMD_PARAM_N; p++) {
    this->_param[p] = param[p];
  }
  this->_start_ms = 0;
}

void Cmd::start(unsigned long start_ms) {
  this->_start_ms = start_ms;
}
void Cmd::end() {
  this->_cmd = CMD_NULL;
}
void Cmd::loop(unsigned long cur_ms) {
  if ( this->is_working() ) {
    Serial.println("!? Cmd[" + String(this->_cmd, HEX)
                   + "::loop(" + String(cur_ms) + ")");
    delay(1000);
  }
}

cmd_t Cmd::get_cmd() {
  return this->_cmd;
}
param_t *Cmd::get_param() {
  return this->_param;
}
param_t Cmd::get_param(int param_i) {
  return this->_param[param_i];
}

boolean Cmd::is_null() {
  return this->_cmd == CMD_NULL;
}
boolean Cmd::is_working() {
  return this->_cmd != CMD_NULL;
}

String Cmd::toString() {
  String str = "";
  str += String(this->_cmd, HEX) + ":";
  for (int p=0; p < CMD_PARAM_N; p++) {
    str += String(this->_param[p]) + " ";
  }
  return str;
}
//============================================================================
CmdSleep::CmdSleep(NixieArray *nxa, cmd_t cmd,
                   param_t param[CMD_PARAM_N]) :
  Cmd::Cmd(nxa, cmd, param) {
}

void CmdSleep::start(unsigned long start_ms) {
  Cmd::start(start_ms);

  this->_interval_ms = (unsigned long)this->get_param(0);
}

void CmdSleep::loop(unsigned long cur_ms) {
  if ( cur_ms - this->_start_ms >= this->_interval_ms ) {
    this->end();
  } else {
    delay(CmdSleep::SLEEP_DELAY);
  }
}
//============================================================================
CmdSetDigit::CmdSetDigit(NixieArray *nxa, cmd_t cmd,
                         param_t param[CMD_PARAM_N]) :
  Cmd::Cmd(nxa, cmd, param) {
}

void CmdSetDigit::start(unsigned long start_ms) {
  Cmd::start(start_ms);

  uint8_t nixie_i = (uint8_t)this->get_param(0);
  uint8_t digit_i = (uint8_t)this->get_param(1);
  uint8_t blightness = (uint8_t)this->get_param(2);

  this->_nxa->set_num_blightness(nixie_i, digit_i, blightness);

  this->end();
}
//============================================================================
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
