/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "netMgr.h"

boolean NetMgr::netIsAvailable() {
  return this->net_is_available;
}

netmgr_mode_t NetMgr::curMode() {
  return this->cur_mode;
}

void NetMgr::loop() {
  String myname = "NetMgr::loop";
  
  this->_loop_count++;

  switch (this->cur_mode) {
  case MODE_START:
    Serial.println(myname + "> MODE_START");

    this->conf_data.load();
    this->ssid = this->conf_data.ssid;
    this->ssid_pw = this->conf_data.ssid_pw;
    Serial.printf("%s> ssid|%s|%s|\n",
                  myname.c_str(), this->ssid.c_str(), this->ssid_pw.c_str());

    // WiFi.begin()
    delay(100);
    this->cur_mode = MODE_TRY_WIFI;
    this->_loop_count = 0;
    break;

  case MODE_TRY_WIFI:
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(myname + "> ");
      Serial.println(WiFi.localIP());

      this->net_is_available = true;
      this->cur_mode = MODE_NULL;
      Serial.println(myname + "> ==> MODE_NULL");
    }

    if (this->_loop_count > WIFI_TRY_COUNT_MAX) {
      Serial.println(myname + "> WiFi faild");

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      this->cur_mode = MODE_SVR_INIT;
      break;
    }
    break;

  case MODE_SVR_INIT:
    Serial.printf(" ");
    break;

  case MODE_SVR_RUN:
    break;

  default:
    break;
  } // switch
}
