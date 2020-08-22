/*
 * (c) 2020 Yoichi Tanibayashi
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

  static unsigned long prev_n = 0;
  static unsigned long a = 2000; // ms
  unsigned long cur_n = cur_ms / a % 10;

  if ( cur_n != prev_n ) {
    param_t *param = new param_t[CMD_PARAM_N];

    param[0] = 0;
    param[1] = prev_n;
    param[2] = 0;
    this->_cmd_q->put(CMD_SET_DIGIT, param);

    param[0] = 0;
    param[1] = cur_n;
    param[2] = BLIGHTNESS_MAX;
    this->_cmd_q->put(CMD_SET_DIGIT, param);

    param[0] = 1;
    param[1] = cur_n;
    param[2] = 200;
    this->_cmd_q->put(CMD_FADE_IN, param);

    param[0] = 2;
    param[1] = cur_n;
    param[2] = 300;
    this->_cmd_q->put(CMD_FADE_OUT, param);

    for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
      param[0] = 3;
      param[1] = d;
      if ( d == cur_n ) {
        param[2] = BLIGHTNESS_MAX;
      } else {
        param[2] = 0;
      }
      this->_cmd_q->put(CMD_SET_DIGIT, param);
    } // for(d)

    param[0] = 3;
    param[1] = 200;
    this->_cmd_q->put(CMD_FOG_IN, param);

    param[0] = 4;
    param[1] = cur_n;
    param[2] = 200;
    this->_cmd_q->put(CMD_FOG_OUT, param);

    prev_n = cur_n;
  }
} // ModeTest1::loop()
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
