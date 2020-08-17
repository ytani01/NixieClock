/*
 *
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H
#include <Arduino.h>
#include "NixieTubeArray.h"
#include "CmdQueue.h"

class ModeBase {
 public:
  virtual void setup(int idx, NixieTubeArray *nxa, CmdQueue *cmd_q);
  virtual void loop(unsigned long cur_ms);

 protected:
  String         _name;
  int            _idx;
  NixieTubeArray *_nxa;
  CmdQueue       *_cmd_q;
};
#endif // MODE_BASE_H
