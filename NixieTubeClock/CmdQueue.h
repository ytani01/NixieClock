/*
 *
 */
#ifndef CMD_QUEUE_H
#define CMD_QUEUE_H
#include <Arduino.h>
#include <string.h>

#define CMD_NULL       0
#define CMD_ON         1
#define CMD_OFF        2
#define CMD_NX_NUM     

#define CMD_Q_SIZE   255
#define CMD_Q_PARAM_N 10

struct CmdQueueEnt {
  uint8_t _cmd;
  uint8_t _param[CMD_Q_PARAM_N];
};

class CmdQueue {
 public:
  CmdQueue() {};
  
  void put(uint8_t cmd, uint8_t *param);
  CmdQueueEnt *get();
  
  int len();
  void printall();
  
 private:
  CmdQueueEnt _ent[CMD_Q_SIZE];
  int _head = 0;
  int _tail = 0;
};

#endif // CMD_QUEUE_H
