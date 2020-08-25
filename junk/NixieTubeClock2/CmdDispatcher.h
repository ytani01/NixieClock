/*
 * (c) 2020 Yoichi Tanibayashi
 */
#ifndef CMD_DISPATCHER_H
#define CMD_DISPATCHER_H

#include <Arduino.h>
#include "NixieArray.h"
#include "Cmd.h"
#include "CmdQueue.h"

#define CMD_WORKER_N   128

class CmdDispatcher {
 public:
  CmdDispatcher() {};

  void setup(NixieArray *nxa, CmdQueue *cmd_q);
  int get_free_worker();
  int set_worker_cmd(cmd_t cmd, param_t param[CMD_PARAM_N]);
  void reset_worker(int wi);

  void loop(unsigned long cur_ms);

 private:
  NixieArray *_nxa;
  CmdQueue   *_cmd_q;
  Cmd        *_worker[CMD_WORKER_N];
  int        _wi;  // worker index
}; // class CmdDispatcher

#endif // CMD_DISPATCHER_H
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
