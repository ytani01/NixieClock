/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "netMgr.h"

NetMgr::NetMgr() {
  this->net_is_available = false;
  this->cur_mode = NetMgr::MODE_START;
  this->ap_ip = IPAddress(192,168,1,100);
  this->ap_netmask = IPAddress(255,255,255,0);
} // constructor

boolean NetMgr::netIsAvailable() {
  return this->net_is_available;
}

netmgr_mode_t NetMgr::curMode() {
  return this->cur_mode;
}

void NetMgr::loop() {
}
