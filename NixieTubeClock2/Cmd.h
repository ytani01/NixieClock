/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef CMD_H
#define CMD_H

#include <Arduino.h>
#include "NixieArray.h"

#define CMD_NULL	0x00 // param: NONE
#define CMD_SLEEP	0x01 // param: interval_ms
#define CMD_SET_DIGIT	0x11 // param: num_i, digit_i, blightness
#define CMD_SET_COLON	0x12 // param: colon_rl, colon_dot_i, blightness
#define CMD_FADE_IN     0x21 // param: num_i, digit_i, delay_ms
#define CMD_FADE_OUT    0x22 // param: num_i, digit_i, delay_ms
#define CMD_FOG_IN      0x23 // param: num_i, delay_ms
#define CMD_FOG_OUT     0x24 // param: num_i, digit_i, delay_ms

#define CMD_N           0xff
#define CMD_PARAM_N     32

typedef uint8_t cmd_t;
typedef long    param_t;
//============================================================================
class Cmd {
 public:
  Cmd(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);

  virtual void start(unsigned long start_ms);
  virtual void end();
  virtual void loop(unsigned long cur_ms);

  virtual cmd_t get_cmd();
  virtual param_t *get_param();
  virtual param_t get_param(int param_i);

  virtual boolean is_null();
  virtual boolean is_working();

  virtual String toString();

 protected:
  NixieArray   *_nxa;
  cmd_t         _cmd = CMD_NULL; // 実行中は、CMD_NULL以外
  param_t       _param[CMD_PARAM_N];
  unsigned long _start_ms;
}; // class Cmd
//============================================================================
class CmdSleep : public Cmd {
 public:
  static const unsigned long SLEEP_DELAY = 1; // ms

  CmdSleep(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  unsigned long _cur_ms;
  unsigned long _interval_ms;
}; // class CmdSleep
//============================================================================
class CmdSetDigit : public Cmd {
 public:
  CmdSetDigit(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
}; // class CmdSetDigit
//============================================================================
class CmdFadeIn : public Cmd {
 public:
  CmdFadeIn(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  uint8_t _num_i;
  uint8_t _digit_i;
  unsigned long _delay_ms;

  uint8_t _blightness;
  unsigned long _prev_ms;
}; // class CmdFadeIn
//============================================================================
class CmdFadeOut : public Cmd {
 public:
  CmdFadeOut(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  uint8_t _num_i;
  uint8_t _digit_i;
  unsigned long _delay_ms;

  uint8_t _blightness;
  unsigned long _prev_ms;
}; // class CmdFadeOut
//============================================================================
class CmdFogIn : public Cmd {
 public:
  CmdFogIn(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  uint8_t _num_i;
  unsigned long _delay_ms;

  uint8_t _blightness[NIXIE_NUM_DIGIT_N];
  unsigned long _prev_ms;
}; // class CmdFogIn
//============================================================================
class CmdFogOut : public Cmd {
 public:
  CmdFogOut(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  uint8_t _num_i;
  uint8_t _digit_i;
  unsigned long _delay_ms;

  uint8_t _blightness[NIXIE_NUM_DIGIT_N];
  unsigned long _prev_ms;
}; // class CmdFogOut
//============================================================================
#endif // CMD_H
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
