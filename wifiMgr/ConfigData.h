/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "SPIFFS.h"

static const char CONFIG_FILE[] = "/config.txt";

/**
 *
 */
class ConfigData {
 public:

  String ssid = "";
  String ssid_pw = "";
  String ntp_svr[3];

  ConfigData();

  int readConfigFile(const char* config_file = CONFIG_FILE);
  int writeConfigFile(const char* config_file = CONFIG_FILE);

  void print();
  
 private:
  
};

#endif // CONFIG_DATA_H
