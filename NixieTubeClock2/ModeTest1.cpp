/*
 *
 */
#include "ModeTest1.h"

void ModeTest1::setup(int idx, NixieArray *nxa, CmdQueue *cmd_q) {
  ModeBase::setup(idx, nxa, cmd_q);

  this->_name = "ModeTest1";

  Serial.print("TestMode1::setup()");
  Serial.print(": _name=" + this->_name);
  Serial.print(", _idx=" + String(this->_idx));
  Serial.println();

  for (int nn=0; nn < NIXIE_NUM_N; nn++) {
    for (int nnd=0; nnd < NIXIE_NUM_DIGIT_N; nnd++) {
      param_t param[CMD_PARAM_N];
      param[0] = nn;
      param[1] = nnd;
      param[3] = 0;
      this->_cmd_q->put(CMD_SET_DIGIT, param);
    } // for(nnd)
  } // for(nd)

  this->_cmd_q->print_all();
}

void ModeTest1::loop(unsigned long cur_ms) {
  ModeBase::loop(cur_ms);

  static unsigned long prev_ms = 0;
  
  if (cur_ms / 1000 != prev_ms / 1000) {
    prev_ms = cur_ms;
    Serial.println("ModeTest1::loop(" + String(cur_ms) + ")");

    int prev_n = (prev_ms / 1000) % 10;
    int cur_n = (cur_ms / 1000) % 10;
    param_t *param = new param_t[10];

    param[0] = 0;
    param[1] = prev_n;
    param[2] = 0;
    this->_cmd_q->put(CMD_SET_DIGIT, param);

    param[1] = cur_n;
    param[2] = BLIGHTNESS_MAX;
    this->_cmd_q->put(CMD_SET_DIGIT, param);
  }
}
