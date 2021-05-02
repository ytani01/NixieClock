/**
 * (c) 2021 Yoichi Tanibayashi
 */
#ifndef CONFIG_DATA_H
#define CONFIG_DATA_H

#include "SPIFFS.h"

static const char CONFIG_FILE[] = "/config.txt";

class ConfigData {
 public:

  String ssid = "";
  String ssid_pw = "";

  ConfigData();

  int readConfigFile(const char* config_file);
  int writeConfigFile(const char* config_file);

  void print();
  
 private:
  
};

#endif /* CONFIG_DATA_H */
