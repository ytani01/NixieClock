/*
 *
 */
#ifndef CMD_H
#define CMD_H

#include <Arduino.h>

#define CMD_PARAM_N    32

#define CMD_NULL	0x00 // param: NONE
#define CMD_SLEEP	0x01 // param: msec, start_msec
#define CMD_SET_DIGIT	0x11 // param: nixie_i, digit_i, blightness
#define CMD_SET_COLON	0x12 // param: colon_rl, colon_dot_i, blightness
#define CMD_FADE1_OUT   0x21 // param: nixie_i, digit_i
#define CMD_FADE1_IN    0x22 // param: nixie_i, digit_i

#define CMD_N           0xff

#define CMD_PARAM_N    32

#define CMD_NULL	0x00 // param: NONE
#define CMD_SLEEP	0x01 // param: msec, start_msec
#define CMD_SET_DIGIT	0x11 // param: nixie_i, digit_i, blightness
#define CMD_SET_COLON	0x12 // param: colon_rl, colon_dot_i, blightness
#define CMD_FADE1_OUT   0x21 // param: nixie_i, digit_i
#define CMD_FADE1_IN    0x22 // param: nixie_i, digit_i

#define CMD_N           0xff

class Cmd {
 public:
  virtual void setup();
  virtual void start(uint8_t param[CMD_PARAM_N]);
  virtual void end();
  virtual void loop(unsigned long cur_ms);

 protected:
  boolean _working;
}; // class Cmd

class CmdSleep : public Cmd {
 public:
  void start(uint8_t param[CMD_PARAM_N]);
  void loop(unsigned long cur_ms);

 private:
  unsigned long _ms;
}; // class CmdSleep
//============================================================================
#endif // CMD_H
