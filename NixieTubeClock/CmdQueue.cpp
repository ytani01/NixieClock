/*
 *
 */
#include "CmdQueue.h"

//============================================================================
void CmdQueue::put(uint8_t cmd, uint8_t *param) {
  if ( this->len() >= CMD_Q_SIZE ) {
    Serial.println("QmdQueue::put> full");
    return;
  }

  this->_ent[this->_tail]._cmd = cmd;
  //XXX
    
}

CmdQueueEnt *CmdQueue::get() {
}

int CmdQueue::len() {
  uint8_t len = this->_tail - this->_head;
  if ( this->_tail < this->_head ) {
    len = this->_tail + CMD_Q_SIZE - this->_head;
  }
  return len;
}

void CmdQueue::printall() {
  int qi = this->_head;
  
  while ( qi != this->_tail ) {
    Serial.print("[" + String(qi) + "]");
    Serial.print(String(this->_ent[qi]._cmd) + " ");
    qi++;
    if ( qi >= CMD_Q_SIZE ) {
      qi=0;
    }
  }
  Serial.println();
}
