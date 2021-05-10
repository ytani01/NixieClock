/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "ConfigData.h"

/**
 *
 */
ConfigData::ConfigData() {
} // ConfigData::ConfigData()

/**
 *
 */
String read_line(File file) {
  String line = file.readStringUntil('\n');
  line.trim();
  return line;
}

/**
 *
 */
int ConfigData::load(const char* config_file) {
  const char* myname = "ConfigData::load";

  if(!SPIFFS.begin(true)) {
    Serial.println("SPIFFS mout failed");
    return -1;
  }

  File file = SPIFFS.open(config_file, "r");
  if (!file) {
    Serial.printf("%s> %s: open faild\n", myname, config_file);
    this->ssid = "";
    this->ssid_pw = "";
    return -1;
  }

  this->ssid = read_line(file);
  this->ssid.trim();
  Serial.printf("%s> SSID: %s\n", myname, this->ssid.c_str());
  
  this->ssid_pw = read_line(file);
  this->ssid_pw.trim();
  Serial.printf("%s> SSID PW: %s\n", myname, this->ssid_pw.c_str());

  file.close();
  return 0;
} // ConfigData::load

/**
 *
 */
int ConfigData::save(const char* config_file) {
  const char* myname = "ConfigData::save";

  this->ssid.trim();
  this->ssid_pw.trim();

  Serial.printf("%s> SSID=%s\n", myname, ssid.c_str());
  Serial.printf("%s> SSID PW=%s\n", myname, ssid_pw.c_str());

  File file = SPIFFS.open(config_file, "w");
  if (!file) {
    Serial.printf("%s> ERROR open failed: %s\n", myname, config_file);
    return -1;
  }
  
  file.println(this->ssid);
  file.println(this->ssid_pw);
  file.close();

  Serial.printf("%s> wrote: %s\n", myname, config_file);
  delay(100);
  return 0;
} // ConfigData::save()

/**
 *
 */
void ConfigData::print() {
  Serial.printf("SSID: %s\n", this->ssid.c_str());
  Serial.printf("SSID PW: %s\n", this->ssid_pw.c_str());
} // ConfigData::print()
