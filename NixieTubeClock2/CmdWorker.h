/*
 * (c) 2020 Yoichi Tanibayashi
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

  void print();

 private:
  NixieArray *_nxa;
  CmdQueue   *_cmd_q;
  Cmd        *_cmd;
}; // class CmdWorker

#endif // CMD_WORKER_H
