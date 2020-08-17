/*
 *
 */
#include "CmdQueue.h"

//============================================================================
void CmdQueue::init(NixieTubeArray *nxa) {
  this->cur_cmd.cmd = CMD_NULL;

  for (int ent=0; ent < CMD_Q_POOL_SIZE; ent++) {
    this->_ent[ent].cmd = CMD_NULL;
  } // for(ent)

  this->_head = this->_tail = 0;

  this->_nxa = nxa;
}

void CmdQueue::put(uint8_t cmd, uint8_t param[CMD_Q_PARAM_N]) {
  if (this->_ent[this->_tail].cmd != CMD_NULL ) {
    Serial.println("CmdQueue::put> full");
    return;
  }

  this->_ent[this->_tail].cmd = cmd;
  for (int p=0; p < CMD_Q_PARAM_N; p++) {
    this->_ent[this->_tail].param[p] = param[p];
  } // for(p)

  this->_tail = (this->_tail + 1) % CMD_Q_POOL_SIZE;
}

CmdEnt *CmdQueue::get() {
  if ( this->_ent[this->_head].cmd == CMD_NULL ) {
    Serial.println("CmdQueue::get> empty");
    return (CmdEnt *)NULL;
  }

  this->cur_cmd.cmd = this->_ent[this->_head].cmd;
  for (int p=0; p < CMD_Q_PARAM_N; p++) {
    this->cur_cmd.param[p] = this->_ent[this->_head].param[p];
  } // for(p)

  this->_head = (this->_head + 1) % CMD_Q_POOL_SIZE;
  return &(this->cur_cmd);
}

void CmdQueue::done() {
  this->cur_cmd.cmd = CMD_NULL;
}

void CmdQueue::set_digit(uint8_t nixie_i, uint8_t digit_i,
                         uint8_t blightness) {
  uint8_t param[NIXIE_TUBE_DIGIT_N];
  param[0] = nixie_i;
  param[1] = digit_i;
  param[2] = blightness;
  this->put(CMD_SET_DIGIT, param);
}

void CmdQueue::loop() {
  while (this->cur_cmd.cmd != CMD_NULL || this->get() != (CmdEnt *)NULL) {
    this->print_cur();
    
    switch (this->cur_cmd.cmd) {
    case CMD_SET_DIGIT:
      this->_nxa->set_tube_blightness((uint8_t)this->cur_cmd.param[0],
				      (uint8_t)this->cur_cmd.param[1],
				      (uint8_t)this->cur_cmd.param[2]);
      this->done();
      break;

    case CMD_SET_COLON:
      this->_nxa->set_colon_blightness((uint8_t)this->cur_cmd.param[0],
				       (uint8_t)this->cur_cmd.param[1]);
      this->done();
      break;

    default:
      Serial.println("this->cur_cmd=0x"
                     + String(this->cur_cmd.cmd, HEX) + "?");
      break;
    } // switch
  } // while
}
 
void CmdQueue::print_cur() {
  Serial.print("cmd(0x" + String(this->cur_cmd.cmd, HEX) + ")");
  for (int p=0; p < CMD_Q_PARAM_N; p++) {
    Serial.print(String(this->cur_cmd.param[p]) + " ");
  }
  Serial.println();
}

void CmdQueue::print_all() {
  int qi = this->_head;
  
  while ( this->_ent[qi].cmd != CMD_NULL ) {
    Serial.print("[" + String(qi) + "]");
    Serial.print(String(this->_ent[qi].cmd) + ":");
    for (int p=0; p < CMD_Q_PARAM_N; p++) {
      Serial.print(String(this->_ent[qi].param[p]) + " ");
    } // for(p)
    Serial.println();

    qi = (qi + 1) % CMD_Q_POOL_SIZE;
    if ( qi == this->_tail ) break;
  } // while
}
