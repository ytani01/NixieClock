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

}

void ModeTest1::init() {
  ModeBase::init();

  param_t param[CMD_PARAM_N];

  for (int num_i=0; num_i < NIXIE_NUM_N; num_i++) {
    param[0] = num_i;
    this->_cmd_q->put(CMD_CLEAR_NUM, param);
  } // for(nd)

  this->_cmd_q->print_all();
}

void ModeTest1::loop(unsigned long cur_ms) {
  ModeBase::loop(cur_ms);

  static unsigned long interval = 2000; // ms
  static unsigned long num = 0;

  static unsigned long prev_n = 0;
  unsigned long cur_n = cur_ms / interval;

  if ( cur_n != prev_n ) {
    param_t param[CMD_PARAM_N];

    param[0] = 0; // num_i
    this->_cmd_q->put(CMD_CLEAR_NUM, param);
    param[1] = num; // digit_i
    param[2] = 0; // blightness
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);

    param[0] = 0; // num_i
    param[1] = num; // digit_i
    param[2] = BLIGHTNESS_MAX; // blightness
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);

    param[0] = 1;
    param[1] = num;
    param[2] = 200;
    this->_cmd_q->put(CMD_FADE_IN_NUM_DIGIT, param);

    param[0] = 2;
    param[1] = num;
    param[2] = 300;
    this->_cmd_q->put(CMD_FADE_OUT_NUM_DIGIT, param);

    param[0] = 3;
    this->_cmd_q->put(CMD_CLEAR_NUM, param);
    param[1] = num;
    param[2] = BLIGHTNESS_MAX;
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);

    param[0] = 3;
    param[1] = 200;
    this->_cmd_q->put(CMD_FOG_IN_NUM_DIGIT, param);

    param[0] = 4;
    param[1] = num;
    param[2] = 200;
    this->_cmd_q->put(CMD_FOG_OUT_NUM_DIGIT, param);

    param[0] = 5;
    //this->_cmd_q->put(CMD_CLEAR_NUM, param);
    param[1] = 0;
    param[2] = BLIGHTNESS_MAX;
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);
    param[1] = 1;
    param[2] = BLIGHTNESS_MAX;
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);
    param[1] = 4;
    param[2] = BLIGHTNESS_MAX;
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);
    param[1] = 6;
    param[2] = BLIGHTNESS_MAX / 2;
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);
    param[1] = 8;
    param[2] = BLIGHTNESS_MAX / 2;
    this->_cmd_q->put(CMD_SET_NUM_DIGIT, param);
    
    prev_n = cur_n;
    num = (num + 1) % 10;
  }
} // ModeTest1::loop()

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
