/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "CmdWorker.h"
//============================================================================
void CmdWorker::setup(NixieArray *nxa, CmdQueue *cmd_q) {
  this->_nxa = nxa;
  this->_cmd_q = cmd_q;
}

boolean CmdWorker::set_digit(uint8_t num_i, uint8_t digit_i,
                             uint8_t blightness) {
}

void CmdWorker::loop(unsigned long cur_ms) {
  if (! this->is_working()) {
    if (! this->_cmd_q->get(&(this->_cmd), this->_param)) {
      return;
    }
  }

  //this->_cmd[cmd].loop(cur_ms);
}

boolean CmdWorker::is_working() {
  return this->_cmd != CMD_NULL;
}

void CmdWorker::done() {
  this->_cmd = CMD_NULL;
}

void CmdWorker::print() {
  Serial.println("_cmd=" + String(this->_cmd, HEX));
}
