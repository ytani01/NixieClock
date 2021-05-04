/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "NetSetup.h"

static String class_name = "NetSetup";

/**
 *
 */
NetSetup::NetSetup() {
} // NetSetup::NetSetup()

/**
 *
 */
uint8_t NetSetup::scanSSID() {
  String msg_hdr = class_name + "::scanSSID";

  Serial.printf("%s> scan start ..\n", msg_hdr.c_str());
  this->ssid_n = WiFi.scanNetworks();
  Serial.printf("%s> %d SSIDs found\n", msg_hdr.c_str(), this->ssid_n);

  if (this->ssid_n <= 0) {
    return this->ssid_n;
  }
  
  if (this->ssid_n > NetSetup::SSID_N_MAX) {
    this->ssid_n = NetSetup::SSID_N_MAX;
  }

  for (int i=0; i < this->ssid_n; i++) {
    this->_ssid_ent[i].set(WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
  } // for(i)

  return this->ssid_n;
} // NetSetup::scanWiFi()

/**
 *
 */
String NetSetup::ssid(int i, boolean long_flag) {
  if ( long_flag ) {
    return this->_ssid_ent[i].toString();
  } else {
    return this->_ssid_ent[i].ssid();
  }
} // NetSetup::ssid()
