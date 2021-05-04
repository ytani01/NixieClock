/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef NET_SETUP_H
#define NET_SETUP_H

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "ConfigData.h"
#include "SSIDent.h"

/**
 *
 */
class NetSetup {
 public:
  static const unsigned int SSID_N_MAX = 30;

  int ssid_n = 0;

  NetSetup();

  uint8_t scanSSID();
  String  ssid(int i, boolean long_flag = false);

 private:
  SSIDent _ssid_ent[SSID_N_MAX];
};

#endif // NET_SETUP_H
