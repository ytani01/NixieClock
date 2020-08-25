/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef MODE_TEST1_H
#define MODE_TEST1_H
#include "ModeBase.h"

class ModeTest1 : public ModeBase {
 public:
  void setup(int idx, NixieArray *nxa);
  void init();
  void loop(unsigned long cur_ms);

 private:
  int _digit = 0;
};
#endif // MODE_TEST1_H
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
