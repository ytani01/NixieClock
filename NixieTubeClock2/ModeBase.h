/*
 *
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H
#include <Arduino.h>
#include "NixieArray.h"
#include "CmdQueue.h"

class ModeBase {
 public:
  // setup()内での初期化
  virtual void setup(int idx, NixieArray *nxa, CmdQueue *cmd_q);

  // モード変更時の初期化: loop()内
  virtual void init();

  // loop()内での処理
  virtual void loop(unsigned long cur_ms);

 protected:
  String     _name;
  int        _idx;
  NixieArray *_nxa;
  CmdQueue   *_cmd_q;
};
#endif // MODE_BASE_H
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
