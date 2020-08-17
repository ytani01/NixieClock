/*
 *
 * ring buffer: CmdEnt
 *
 *           _head          _tail
 *             |              |
 *             v              v
 *         +--+--+--+--+--+--+--+--+--+--+--+--+--+
 * CmdEnt  |  |  |  |  |  |  |  |  |  |  |  |  |  |
 *         +--+--+--+--+--+--+--+--+--+--+--+--+--+
 *           0  1  2  3  4  ...                 (CMD_Q_POOL_SIZE-1)
 */
#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H
#include <Arduino.h>
#include <string.h>
#include "NixieTubeArray.h"

#define CMD_NULL	0x00 // param: NONE
#define CMD_SLEEP	0x01 // param: msec, start_msec
#define CMD_SET_DIGIT	0x11 // param: nixie_i, digit_i, blightness
#define CMD_SET_COLON	0x12 // param: colon_rl, colon_dot_i, blightness

#define CMD_Q_POOL_SIZE 255
#define CMD_Q_PARAM_N    10

struct CmdEnt {
  uint8_t cmd;
  uint8_t param[CMD_Q_PARAM_N];
};

class CmdQueue {
 public:
  CmdEnt cur_cmd;

  CmdQueue() {};

  void init(NixieTubeArray *nxa);
  
  void put(uint8_t cmd, uint8_t param[CMD_Q_PARAM_N]);
  CmdEnt *get(); // get and remove
  
  void loop();

  void print_cur();
  void print_all();
  
 private:
  CmdEnt _ent[CMD_Q_POOL_SIZE];
  int _head = 0;
  int _tail = 0;

  NixieTubeArray *_nxa;
};

#endif // CMD_QUEUE_H
