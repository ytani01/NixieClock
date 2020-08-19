/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "CmdWorker.h"

CmdSleep cmdSleep;
Cmd cmdNull;

Cmd *curCmd = &cmdNull;

//============================================================================
void CmdWorker::setup(NixieArray *nxa, CmdQueue *cmd_q) {
  this->_nxa = nxa;
  this->_cmd_q = cmd_q;
}

void CmdWorker::loop(unsigned long cur_ms) {
  while (curCmd->is_null()) {
    // 実行中のコマンドが無い場合
    cmd_t   cmd;
    param_t param[CMD_PARAM_N];
    
    // コマンド・キューから、取り出す
    if (! this->_cmd_q->get(&cmd, param)) {
      // コマンド・キューがからの場合は、return
      return;
    }
    Serial.println("cmd=" + String(cmd, HEX));
    
    // コマンドに応じてcurCmdオブジェクトを切り替える
    switch (cmd) {
    case CMD_SLEEP: curCmd = &cmdSleep; break;
    default:
      Serial.println("invalic cmd=" + String(cmd, HEX));
      curCmd = &cmdNull;
      return;
    } // switch
    
    // コマンドの初期化、引数の受け渡しをして、実行開始
    curCmd->init(this->_nxa, cmd, param);
    curCmd->start();
    if (! curCmd->is_working()) {
      // start()だけで完了した場合、curCmdをリセット
      curCmd = &cmdNull;
    }
  } while()

  curCmd->loop(cur_ms);
  if (! curCmd->is_working()) {
    // コマンドが完了した場合、curCmdをリセット
    curCmd = &cmdNull;
  }
}

void CmdWorker::print() {
  String str = "";

  str += curCmd->toString() + ":";
    
  Serial.println(str);
}
