/*
 *
 */
#ifndef MODE_TEST1_H
#define MODE_TEST1_H
#include "ModeBase.h"

class ModeTest1 : public ModeBase {
 public:
  void setup(int idx, NixieArray *nxa, CmdQueue *cmd_q);
  void loop(unsigned long cur_ms);
};
#endif // MODE_TEST1_H
