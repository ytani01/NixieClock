/*
 *
 */
#ifndef CMD_WORKER_H
#define CMD_WORKER_H

#include <Arduino.h>
#include "NixieArray.h"
#include "Cmd.h"
#include "CmdQueue.h"

class CmdWorker {
 public:
  CmdWorker() {};

  void setup(NixieArray *nxa, CmdQueue *cmd_q);

  void loop(unsigned long cur_ms);
  boolean is_working();
  void done();

  void print();

  boolean set_digit(uint8_t num_i, uint8_t digit_i, uint8_t blightness);

 private:
  uint8_t    _cmd = CMD_NULL;
  uint8_t    _param[CMD_PARAM_N];
  NixieArray *_nxa;
  CmdQueue   *_cmd_q;
  Cmd        _cmd_obj[CMD_N];
}; // class CmdWorker

#endif // CMD_WORKER_H
