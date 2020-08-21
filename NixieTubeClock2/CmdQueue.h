/*
 *
 * ring buffer: CmdEnt[]
 *
 *            _head          _tail
 *             |              |
 *             v              v
 *         +--+--+--+--+--+--+--+--+--+--+--+--+--+
 * CmdEnt  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *         +--+--+--+--+--+--+--+--+--+--+--+--+--+
 *          0  1  2  3  4  ...                  (CMD_Q_POOL_SIZE-1)
 */
#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H
#include <Arduino.h>
#include <string.h>
#include "Cmd.h"

#define CMD_Q_POOL_SIZE 512

struct CmdEnt {
  cmd_t   cmd;
  param_t param[CMD_PARAM_N];
};

class CmdQueue {
 public:
  CmdQueue() {};

  void setup();
  
  boolean put(cmd_t cmd, param_t param[CMD_PARAM_N]);
  boolean get(cmd_t *cmd, param_t param[CMD_PARAM_N]);

  int get_head();
  int get_tail();

  void print_all();
  
 private:
  CmdEnt _ent[CMD_Q_POOL_SIZE];
  int _head = 0;
  int _tail = 0;
}; // class CmdQueue

#endif // CMD_QUEUE_H
