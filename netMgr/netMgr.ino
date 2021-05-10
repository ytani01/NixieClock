/**
 * (c) 2021 Yoichi Tanibayashi
 */

#include "SPIFFS.h"

#include "ConfigData.h"
#include "SSIDent.h"
#include "NetMgr.h"

#define PIN_LED 2

boolean netAvailable = false;

const unsigned int MODE_START    = 0x00;
const unsigned int MODE_TRY_WIFI = 0x01;
const unsigned int MODE_AP_INIT = 0x10;
const unsigned int MODE_AP_RUN  = 0x11;
const unsigned int MODE_ON       = 0x80;

unsigned int  curMode     = MODE_START;
unsigned long modeStartMs = 0;
unsigned int  loopCount   = 0;

const unsigned int WIFI_TRY_INTERVAL  = 500; // ms
const unsigned int WIFI_TRY_COUNT_MAX = 10;

const unsigned int SVR_LOOP_INTERVAL = 1; // ms

const String AP_SSID_HDR = "NIXIE_CLOCK_";
String AP_SSID = AP_SSID_HDR;
const String AP_SSID_PW  = "xxxxxxxx";

const int DNS_PORT     = 53;
const int WEB_SVR_PORT = 80;

//IPAddress AP_IP(192,168,1,100);
//IPAddress AP_SUBNET(255,255,255,0);
DNSServer dnsServer;
WebServer webServer(WEB_SVR_PORT);

// parameters setting
String ssid = "";
String ssid_pw = "";

const unsigned int SSID_N_MAX = 30;
SSIDent ssidEnt[SSID_N_MAX];

ConfigData confData;

NetMgr netMgr;

/**
 *
 */
unsigned int scanSSID(SSIDent ssid_ent[]) {
  Serial.printf("scanSSID> scan start ..\n");
  int ssid_n = WiFi.scanNetworks();
  Serial.printf("scanSSID> %d SSIDs found\n", ssid_n);

  if (ssid_n <= 0) {
    return 0;
  }

  if (ssid_n > SSID_N_MAX) {
    ssid_n = SSID_N_MAX;
  }

  for (int i=0; i < ssid_n; i++) {
    ssid_ent[i].set(WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
  } // for(i)

  return ssid_n;
}

/**
 *
 */
unsigned int changeMode(unsigned int mode) {
  loopCount = 0;
  curMode = mode;
  modeStartMs = millis();
  Serial.printf("changeMode> modeStartMs=%lu mode=%02X\n",
                modeStartMs, (int)curMode);
  return curMode;
} // changeMode()

/**
 *
 */
String html_header(String title) {
  String html = "";
  html += "<!DOCTYPE html>";
  html += "<html>";
  html += "<head>";

  html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  html += "<meta http-equiv='Pragma' content='no-cache'>";
  html += "<meta http-equiv='Cache-Control' content='no-cache'>";
  html += "<meta http-equiv='Expires' content='0'>";
  html += "<meta http-equiv='Content-type' CONTENT='text/html; charset=utf-8'>";

  html += "<style>";
  html += "a:link, a:visited {";
  html += " background-color: #00AA00;";
  html += " color: white;";
  html += " border: none;";
  html += " padding: 5px 15px;";
  html += " text-align: center;";
  html += " text-decoration: none;";
  html += " display: inline-block;";
  html += "}";
  html += "a:hover, a:active {";
  html += " background-color: #00FF00;";
  html += "}";
  html += "input[type=button], input[type=submit], input[type=reset] {";
  html += " background-color: #0000AA;";
  html += " color: white;";
  html += " text-decoration: none;";
  html += " font-size: large;";
  html += " border: none;";
  html += " padding: 5px 20px;";
  html += " margin: 4px 2px;";
  html += "}";
  html += "</style>";

  html += "</head>";
  html += "<body>"; 
  html += "<h2>" + title + "</h2>";
  return html;
}

//======================================================================
/**
 *
 */
void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
} // setup()

/**
 *
 */
void loop() {
  netmgr_mode_t netmgr_mode;

  loopCount++;
    
  netmgr_mode = netMgr.loop();
  if (netmgr_mode == NetMgr::MODE_WIFI_ON) {
    Serial.println("WiFi connected");
  }
  if (netmgr_mode == NetMgr::MODE_WIFI_OFF) {
    Serial.println("WiFi disconnectd");
    netMgr.cur_mode = NetMgr::MODE_START;
  }

  delay(1000);
}
// Local Variables:
// Mode: c++
// Coding: utf-8-unix
// End:
