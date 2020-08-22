/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef CMD_H
#define CMD_H

#include <Arduino.h>
#include "NixieArray.h"

#define CMD_NULL               0x00 // ()

#define CMD_SET_NUM_DIGIT      0x11 // (num_i, digit_i, blightness)
#define CMD_SET_COLON_DOT      0x12 // (colon_i, dot_i, blightness)

#define CMD_CLEAR_NUM          0x13 // (num_i)
#define CMD_CLEAR_COLON        0x14 // (colon_i)

#define CMD_FADE_IN_NUM_DIGIT  0x21 // (num_i, digit_i, delay_ms)
#define CMD_FADE_IN_COLON_DOT  0x22 // (colon_i, dot_i, delay_ms)
#define CMD_FADE_OUT_NUM_DIGIT 0x23 // (num_i, digit_i, delay_ms)
#define CMD_FADE_OUT_COLON_DOT 0x24 // (colon_i, dot_i, delay_ms)

#define CMD_FOG_IN_NUM_DIGIT   0x31 // (num_i, delay_ms)
#define CMD_FOG_OUT_NUM_DIGIT  0x32 // (num_i, digit_i, delay_ms)

#define CMD_N           0xff
#define CMD_PARAM_N     16

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
class CmdSetNumDigit : public Cmd {
 public:
  CmdSetNumDigit(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
}; // class CmdSetNumDigit
//============================================================================
class CmdClearNum : public Cmd {
 public:
  CmdClearNum(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
}; // class CmdClearNum
//============================================================================
class CmdFadeInNumDigit : public Cmd {
 public:
  CmdFadeInNumDigit(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  uint8_t _num_i;
  uint8_t _digit_i;
  unsigned long _delay_ms;

  uint8_t _blightness;
  unsigned long _prev_ms;
}; // class CmdFadeInNumDigit
//============================================================================
class CmdFadeOutNumDigit : public Cmd {
 public:
  CmdFadeOutNumDigit(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
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
class CmdFogInNumDigit : public Cmd {
 public:
  CmdFogInNumDigit(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
  void start(unsigned long start_ms);
  void loop(unsigned long cur_ms);

 private:
  uint8_t _num_i;
  unsigned long _delay_ms;

  uint8_t _blightness[NIXIE_NUM_DIGIT_N];
  unsigned long _prev_ms;
}; // class CmdFogIn
//============================================================================
class CmdFogOutNumDigit : public Cmd {
 public:
  CmdFogOutNumDigit(NixieArray *nxa, cmd_t cmd, param_t param[CMD_PARAM_N]);
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
