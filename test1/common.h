/**
 * Copyright (c) 2023 Yoichi Tanibayashi
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#include <vector>
#include <Arduino.h>
#include <esp32-hal-log.h>

#define VERSION_STR "0.01"

typedef enum {
              MODE_MAIN,
              MODE_N
} Mode_t;
static const char *MODE_T_STR[] =
  {"MAIN", "MENU", "RESTART", "SET_TEMP_OFFSET", "SCAN_SSID", "SET_SSID"};

typedef struct {
  String msg;
  Mode_t cur_mode;
  //  NetMgrInfo_t *netmgr_info;
  //  NtpTaskInfo_t *ntp_info;
} CommonData_t;

#define _cd this->common_data

#endif // _COMMON_H_
