/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "NetMgr.h"

static const String MY_NAME = "Nixie Clock Setup";

#define PIN_LED 2

NetMgr netMgr;

unsigned int loopCount = 0;

/**
 *
 */
void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  NetMgr::myName = MY_NAME;
} // setup()

/**
 *
 */
void loop() {
  netmgr_mode_t netmgr_mode;

  loopCount++;
    
  netmgr_mode = netMgr.loop();
  switch ( netmgr_mode ) {
  case NetMgr::MODE_WIFI_ON:
    Serial.println("WiFi connected");
    break;

  case NetMgr::MODE_WIFI_OFF:
    Serial.println("WiFi disconnectd");
    netMgr.cur_mode = NetMgr::MODE_START;
    break;

  default:
    return;
  }

  delay(1000);
}
// Local Variables:
// Mode: c++
// Coding: utf-8-unix
// End:
