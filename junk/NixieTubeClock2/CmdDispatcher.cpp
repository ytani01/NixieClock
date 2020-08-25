/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "CmdDispatcher.h"

//============================================================================
void CmdDispatcher::setup(NixieArray *nxa, CmdQueue *cmd_q) {
  this->_nxa = nxa;
  this->_cmd_q = cmd_q;
  param_t *param = new param_t[CMD_PARAM_N];
  
  for (int w=0; w < CMD_WORKER_N; w++) {
    this->_worker[w] = new Cmd(nxa, CMD_NULL, param);
  } // for (w)
  this->_wi = 0;
}

/*
 * returns: workerインデックス
 *   -1: fullの場合
 */
int CmdDispatcher::get_free_worker() {
  int start_i = this->_wi;

  while (this->_worker[this->_wi]->get_cmd() != CMD_NULL) {
    if (this->_wi == start_i) {
      return -1;
    }
    this->_wi = (this->_wi + 1) % CMD_WORKER_N;
  }
  return this->_wi;
}

int CmdDispatcher::set_worker_cmd(cmd_t cmd, param_t param[CMD_PARAM_N]) {
  if ( this->_worker[this->_wi]->is_working() ) {
    // ??
    return -1;
  }

  delete this->_worker[this->_wi]; // 一応
  
  switch (cmd) {
  case CMD_NULL:
    this->_worker[this->_wi] = new Cmd(this->_nxa, cmd, param);
    break;
    
  case CMD_SET_NUM_DIGIT:
    this->_worker[this->_wi] = new CmdSetNumDigit(this->_nxa, cmd, param);
    break;
  case CMD_SET_COLON_DOT:
    break;

  case CMD_CLEAR_NUM:
    this->_worker[this->_wi] = new CmdClearNum(this->_nxa, cmd, param);
    break;
  case CMD_CLEAR_COLON:
    break;

  case CMD_FADE_IN_NUM_DIGIT:
    this->_worker[this->_wi] = new CmdFadeInNumDigit(this->_nxa, cmd, param);
    break;
  case CMD_FADE_IN_COLON_DOT:
    break;
  case CMD_FADE_OUT_NUM_DIGIT:
    this->_worker[this->_wi] = new CmdFadeOutNumDigit(this->_nxa, cmd, param);
    break;
  case CMD_FADE_OUT_COLON_DOT:
    break;

  case CMD_FOG_IN_NUM_DIGIT:
    this->_worker[this->_wi] = new CmdFogInNumDigit(this->_nxa, cmd, param);
    break;
  case CMD_FOG_OUT_NUM_DIGIT:
    this->_worker[this->_wi] = new CmdFogOutNumDigit(this->_nxa, cmd, param);
    break;

  default:
    Serial.println("! invalid cmd=" + String(cmd, HEX));
    this->_worker[this->_wi] = new Cmd(this->_nxa, CMD_NULL, param);
    return -1;
  } // switch

  int ret_wi = this->_wi;
  this->_wi = (this->_wi + 1) % CMD_WORKER_N;

  return ret_wi;
}

void CmdDispatcher::reset_worker(int wi) {
  param_t param[CMD_PARAM_N];

  delete this->_worker[wi];
  this->_worker[wi] = new Cmd(this->_nxa, CMD_NULL, param);
}

void CmdDispatcher::loop(unsigned long cur_ms) {
  // コマンド・キューから get & start
  while ( this->get_free_worker() >= 0 ) {
    // 実行中のコマンドが無い場合
    cmd_t   cmd;
    param_t param[CMD_PARAM_N];
    
    // コマンド・キューから、取り出す
    if (! this->_cmd_q->get(&cmd, param)) {
      break;      // キューが空の場合は、取出し終了
    }

    // コマンドに応じてworkerオブジェクトを設定する
    int wi;
    if ( (wi = this->set_worker_cmd(cmd, param)) >= 0 ) {
      this->_worker[wi]->start(cur_ms);  // コマンド実行開始
      if (! this->_worker[wi]->is_working() ) {
        // start()だけで完了した場合、リセット
        this->reset_worker(wi);
      }
    }
  } // while

  // 実行中のworkerのloop()を実行
  for (int w=0; w < CMD_WORKER_N; w++) {
    if ( this->_worker[w]->is_working() ) {
      this->_worker[w]->loop(cur_ms);
    }
  } // for (w)
}

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
