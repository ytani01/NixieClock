/**
 * Copyright (c) 2022 Yoichi Tanibayashi
 */
#include "MainMode.h"

/** constructor
 *
 */
MainMode::MainMode(String name, CommonData_t *common_data)
  : ModeBase(name, common_data) {

  strcpy(this->mac_addr_str, get_mac_addr_String().c_str());
  log_i("mac_addr_str=\"%s\"", this->mac_addr_str);
} // MainMode::MainMode()

/**
 *
 */
Mode_t MainMode::btnCb_Mode(ButtonInfo_t *bi) {
  if ( bi->click_count > 0 ) {
    return MODE_MAIN;
  }
  return MODE_N;
} // MainMode::reBtn_cb()

/**
 *
 */
void MainMode::display(Display_t *disp) {
} // MainMode::display()
