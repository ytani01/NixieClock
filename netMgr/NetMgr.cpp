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
  case MODE_NULL:
    break;
    
  case MODE_START:
    Serial.println(myname + "> MODE_START");

    uint8_t baseMac[6];
    esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
    Serial.printf("%s> Mac Addr=%02X:%02X:%02X:%02X:%02X:%02X\n",
                  myname.c_str(),
                  baseMac[0], baseMac[1], baseMac[2],
                  baseMac[3], baseMac[4], baseMac[5]);

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
    Serial.println(myname + "> MODE_SVR_INIT");

    // XXX
    //WiFi.mode(WIFI_AP);
    //Serial.printf("WiFi.softAP(%s) .. ", this->ap_ssid.c_str());

    this->cur_mode = MODE_SVR_RUN;
    break;

  case MODE_SVR_RUN:
    // XXX
    //dns_svr.processNextRequest();
    //web_svr.handleClient();

    this->cur_mode = MODE_NULL;
    break;

  default:
    Serial.println(myname + "> unknown mode ???");
    break;
  } // switch
}
