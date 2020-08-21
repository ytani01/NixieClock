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
}

/*
 * returns: workerインデックス
 *   -1: fullの場合
 */
int CmdDispatcher::get_free_worker() {
  for (int i=0; i < CMD_WORKER_N; i++) {
    if ( this->_worker[i]->get_cmd() == CMD_NULL ) {
      return i;
    }
  } // for (i)
  return -1;
}

cmd_t CmdDispatcher::set_worker_cmd(int wi, cmd_t cmd,
                                    param_t param[CMD_PARAM_N]) {
  delete this->_worker[wi];

  switch (cmd) {
  case CMD_NULL:
    this->_worker[wi] = new Cmd(this->_nxa, cmd, param);
    break;
  case CMD_SLEEP:
    this->_worker[wi] = new CmdSleep(this->_nxa, cmd, param);
    break;
  case CMD_SET_DIGIT:
    this->_worker[wi] = new CmdSetDigit(this->_nxa, cmd, param);
    break;
  default:
    Serial.println("! invalid cmd=" + String(cmd, HEX));
    this->_worker[wi] = new Cmd(this->_nxa, CMD_NULL, param);
    return CMD_NULL;
  } // switch

  return cmd;
}

void CmdDispatcher::reset_worker(int wi) {
  param_t *param = new param_t[CMD_PARAM_N];

  this->set_worker_cmd(wi, CMD_NULL, param);
}

void CmdDispatcher::loop(unsigned long cur_ms) {
  // コマンド・キューから get & start
  int wi;
  while ( (wi=this->get_free_worker()) >= 0 ) {
    // 実行中のコマンドが無い場合
    cmd_t   cmd;
    param_t param[CMD_PARAM_N];
    
    // コマンド・キューから、取り出す
    if (! this->_cmd_q->get(&cmd, param)) {
      break;      // キューが空の場合は、取出し終了
    }
    
    // コマンドに応じてworkerオブジェクトを設定する
    if ( this->set_worker_cmd(wi, cmd, param) != CMD_NULL ) {
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
