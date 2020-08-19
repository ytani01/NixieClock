/*
 *
 */
#ifndef CMD_H
#define CMD_H

#include <Arduino.h>
#include "NixieArray.h"

#define CMD_NULL	0x00 // param: NONE
#define CMD_SLEEP	0x01 // param: interval_ms
#define CMD_SET_DIGIT	0x11 // param: nixie_i, digit_i, blightness
#define CMD_SET_COLON	0x12 // param: colon_rl, colon_dot_i, blightness
#define CMD_FADE1_OUT   0x21 // param: nixie_i, digit_i
#define CMD_FADE1_IN    0x22 // param: nixie_i, digit_i

#define CMD_N           0xff
#define CMD_PARAM_N     32

typedef uint8_t cmd_t;
typedef long    param_t;
//============================================================================
class Cmd {
 public:
  Cmd() {};
  virtual void init(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  virtual void start();
  virtual void end();
  virtual boolean is_null();
  virtual boolean is_working();
  virtual void loop(unsigned long cur_ms);
  virtual String toString();

 protected:
  NixieArray *_nxa;
  cmd_t       _cmd = CMD_NULL;
  param_t     _param[CMD_PARAM_N];
  boolean     _working = false;
}; // class Cmd
//============================================================================
class CmdSleep : public Cmd {
 public:
  void start();
  void loop(unsigned long cur_ms);

 private:
  unsigned long _start_ms;
  unsigned long _cur_ms;
  unsigned long _interval_ms;
}; // class CmdSleep
//============================================================================
class CmdSetDigit : public Cmd {
 public:
  void start();
}; // class CmdSetDigit
//============================================================================
#endif // CMD_H
