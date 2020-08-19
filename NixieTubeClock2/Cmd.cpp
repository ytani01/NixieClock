/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "Cmd.h"
//============================================================================
void Cmd::setup() {
}

void Cmd::start(uint8_t param[CMD_PARAM_N]) {
  this->_working = true;
}

void Cmd::end() {
  this->_working = false;
}

void Cmd::loop(unsigned long cur_ms) {
}
//============================================================================
void CmdSleep::start(uint8_t param[CMD_PARAM_N]) {
  Cmd::start(param);
  
}

void CmdSleep::loop(unsigned long cur_ms) {
}
