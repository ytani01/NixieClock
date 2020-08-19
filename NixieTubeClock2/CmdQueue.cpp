/*
 * CmdQueue.cpp
 *
 * (c) 2020 Yoichi Tanibayashi
 */
#include "CmdQueue.h"
//============================================================================
void CmdQueue::setup() {
  for (int ent=0; ent < CMD_Q_POOL_SIZE; ent++) {
    this->_ent[ent].cmd = CMD_NULL;
  } // for(ent)

  this->_head = this->_tail = 0;
}

boolean CmdQueue::put(uint8_t cmd, uint8_t param[CMD_PARAM_N]) {
  if (this->_ent[this->_tail].cmd != CMD_NULL ) {
    Serial.println("CmdQueue::put> full");
    return false;
  }

  this->_ent[this->_tail].cmd = cmd;
  for (int p=0; p < CMD_PARAM_N; p++) {
    this->_ent[this->_tail].param[p] = param[p];
  } // for(p)

  this->_tail = (this->_tail + 1) % CMD_Q_POOL_SIZE;
  return true;
}

boolean CmdQueue::get(uint8_t *cmd, uint8_t param[CMD_PARAM_N]) {
  if ( this->_ent[this->_head].cmd == CMD_NULL ) {
    Serial.println("CmdQueue::get> empty");
    return false;
  }

  *cmd = this->_ent[this->_head].cmd;
  for (int p=0; p < CMD_PARAM_N; p++) {
    param[p] = this->_ent[this->_head].param[p];
  } // for(p)
  this->_ent[this->_head].cmd = CMD_NULL;

  this->_head = (this->_head + 1) % CMD_Q_POOL_SIZE;
  return true;
}

void CmdQueue::print_all() {
  int qi = this->_head;
  
  while (this->_ent[qi].cmd != CMD_NULL) {
    Serial.print("[" + String(qi) + "]");
    Serial.print(String(this->_ent[qi].cmd) + ":");
    for (int p=0; p < CMD_PARAM_N; p++) {
      Serial.print(String(this->_ent[qi].param[p]) + " ");
    } // for(p)
    Serial.println();

    qi = (qi + 1) % CMD_Q_POOL_SIZE;
    if ( qi == this->_tail ) break;
  } // while
}
