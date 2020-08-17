/*
 *
 */
#ifndef MODE_TEST1_H
#define MODE_TEST1_H
#include "ModeBase.h"

class ModeTest1 : public ModeBase {
 public:
  ModeTest1() {};
  
  void setup();
  void loop(unsigned long cur_ms, NixieTubeArray *nxa);
};
#endif // MODE_TEST1_H
