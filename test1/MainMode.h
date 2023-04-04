/**
 * Copyright (c) 2022 Yoichi Tanibayashi
 */
#ifndef _MAIN_MODE_H_
#define _MAIN_MODE_H_

#include "commonlib.h"
#include "ModeBase.h"

/** constructor
 *
 */
class MainMode: public ModeBase {
 public:
  char mac_addr_str[13];

  MainMode(String name, CommonData_t *common_data);

  virtual Mode_t btnCb_Mode(ButtonInfo_t *bi);
  virtual void display(Display_t *disp);

 protected:
}; // class MainMode

#endif // _MAIN_MODE_H_
