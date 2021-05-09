/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef NETMGR_H
#define NETMGR_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

#include "SSIDent.h"
#include "ConfigData.h"

typedef unsigned int netmgr_mode_t;

/**
 *
 */
class NetMgr {
 public:
  static const netmgr_mode_t MODE_NULL     = 0x00;
  static const netmgr_mode_t MODE_START    = 0x01;
  static const netmgr_mode_t MODE_TRY_WIFI = 0x02;
  static const netmgr_mode_t MODE_SVR_INIT = 0x10;
  static const netmgr_mode_t MODE_SVR_RUN  = 0x11;

  static const unsigned int SSID_N_MAX = 30;

  static const unsigned int WIFI_TRY_INTERVAL  = 500; // ms
  static const unsigned int WIFI_TRY_COUNT_MAX = 10;

  static const int DNS_PORT    = 53;
  static const int WEBSVR_PORT = 80;

  boolean net_is_available = false;
  netmgr_mode_t cur_mode = MODE_NULL;
  String ssid = "";
  String ssid_pw = "";
  String ntp_svr[3] = {"ntp.nict.jp", "time.goole.com", ""};
  IPAddress ap_ip;
  IPAddress ap_netmask;
  DNSServer dns_svr;
  WebServer web_svr;

  NetMgr();
  void start();

  boolean netIsAvailable();
  netmgr_mode_t curMode();
  void loop();

 private:
};


#endif // NETMGR_H
