/*
 *
 */
#ifndef MODE_BASE_H
#define MODE_BASE_H
#include <Arduino.h>
#include "NixieTubeArray.h"

class ModeBase {
 public:
  ModeBase() {};

  virtual void setup();
  virtual void loop(unsigned long cur_ms, NixieTubeArray *nxa);
};
#endif // MODE_BASE_H
