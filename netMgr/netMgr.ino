/*
   WIFI_MGR for ESP32
   by kghr labo 2020
*/

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "SPIFFS.h"

#include "ConfigData.h"
#include "NetSetup.h"

#define PIN_LED 2

boolean netAvailable = false;

const unsigned int MODE_INIT = 0x00;
const unsigned int MODE_TRY_WIFI = 0x01;
const unsigned int MODE_SVR_INIT = 0x10;
const unsigned int MODE_SVR = 0x11;
const unsigned int MODE_ON = 0x80;
unsigned int curMode = MODE_INIT;
unsigned long modeStartMs = 0;
unsigned int loopCount = 0;

const unsigned int WIFI_TRY_INTERVAL = 500; // ms
const unsigned int WIFI_TRY_COUNT_MAX = 10;

const unsigned int SVR_LOOP_INTERVAL = 1; // ms

const String SVR_SSID = "NIXIE_CLOCK1";
const String SVR_SSID_PW = "xxxxxxxx";
const int DNS_PORT = 53;
const int WEB_SVR_PORT = 80;

IPAddress AP_IP(192,168,1,100);
IPAddress AP_SUBNET(255,255,255,0);
DNSServer dnsServer;
WebServer webServer(WEB_SVR_PORT);

// parameters setting
String ssid = "";
String ssid_pw = "";

#define SSID_N_MAX 30

// SSIDent
SSIDent ssidEnt[SSID_N_MAX];

NetSetup netSetup;

ConfigData confData;

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
String html_header() {
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
  html += "<h2>WIFI Setup</h2>";
  return html;
}

/**
 *
 */
void handle_top() {
  const char* myname = "handle_top";
  Serial.printf("%s> ssid=%s, ssid_pw=%s\n",
                myname, ssid.c_str(), ssid_pw.c_str());

  String html = html_header();
  html += "<hr>";
  html += "<h2>Current SSID: " + ssid + "</h2>";
  //html += "<h2>ssid_pw: " + maskpasswd(ssid_pw) + "</h2>";
  html += "<a href='/select_ssid'>Select WiFi SSID</a>";
  html += "<hr>";
  html += "<a href='/confirm_reboot'>Reboot</a>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

String ssid_form_str(){
  uint8_t ssid_n = netSetup.scanSSID();
  Serial.println(ssid_n);

  for (int i=0; i < netSetup.ssid_n; i++) {
    Serial.print(netSetup.ssid(i));
    Serial.print(" ");
    Serial.print(String(netSetup.dbm(i)));
    Serial.print(" ");
    Serial.print(netSetup.encType(i));
    Serial.println();
  } // for(i)


  String str = "<form action='/save_ssid' method='GET'>";

  str += "SSID<br />";
  str += "<select name='ssid' id='ssid'>";
  for(int i=0; i < netSetup.ssid_n; i++){
    str += "<option value=" + netSetup.ssid(i) + ">";
    str += netSetup.ssid(i);
    str += " (";
    str += String(netSetup.dbm(i));
    str += ", ";
    str += netSetup.encType(i);
    str += ")";
    str += "</option>\n";
  }
  str += "<option value=" + ssid + ">" + ssid + " (current)</option>\n";
  str += "</select><br />\n";
  str += "<br />";
  str += "Password<br />";
  str += "<input type='password' name='passwd' value='" + ssid_pw + "' />";
  str += "<br>";
  str += "<input type='submit' value='Save' />";
  str += "</form>";
  str += "<br />";
  str += "<script>document.getElementById('ssid').value = '"+ ssid +"';</script>";
  return str;
}

void handle_select_ssid() {
  String html = html_header();
  html += "<hr>";
  html += "<h3>Select WiFi SSID</h3>";
  html += ssid_form_str();
  html += "<hr>";
  html += "<a href='/'>Cancel</a>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

void handle_save_ssid(){
  Serial.println("save_ssid>");

  confData.ssid = webServer.arg("ssid");
  confData.ssid.trim();
  confData.ssid_pw = webServer.arg("passwd");
  confData.ssid_pw.trim();
  confData.print();
 
  confData.save();

  ssid = confData.ssid;
  ssid_pw = confData.ssid_pw;
  
  // 自動転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}

void handle_confirm_reboot() {
  String html = html_header();
  html += "<hr><p>";
  html += "<h3>reboot confirmation</h3><p>";
  html += "Are you sure to reboot?<p>";
  html += "<center><a href='/do_reboot'>YES</a> <a href='/'>no</a></center>";
  html += "<p><hr>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

void handle_do_reboot() {
  String html = html_header();
  html += "<hr>";
  html += "<h3>rebooting</h3>";
  html += "<p>The setting WiFi connection will be disconnected...</p>";
  html += "<hr>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);

  Serial.println("reboot esp32 ..\n");
  digitalWrite(PIN_LED, LOW);
    
  delay(2000);
  ESP.restart();
}

//======================================================================
/**
 *
 */
void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  confData.load();
  ssid = confData.ssid;
  ssid_pw = confData.ssid_pw;
  Serial.printf("setup> ssid=%s, ssid_pw=%s\n", ssid.c_str(), ssid_pw.c_str());
} // setup()

/**
 *
 */
void loop() {
  String ssid, ssid_pw;

  loopCount++;
    
  switch (curMode) {

  case MODE_INIT:
    confData.load();

    ssid = confData.ssid;
    ssid_pw = confData.ssid_pw;

    WiFi.begin(ssid.c_str(), ssid_pw.c_str());

    changeMode(MODE_TRY_WIFI);
    delay(100);
    break;

  case MODE_TRY_WIFI:
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("WiFi connected. IP address: ");
      Serial.println(WiFi.localIP());
      
      netAvailable = true;
      
      changeMode(MODE_ON);
      break;
    }

    if (loopCount >= WIFI_TRY_COUNT_MAX) {
      Serial.printf("WiFi failed\n");

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      changeMode(MODE_SVR_INIT);
      break;
    }

    delay(WIFI_TRY_INTERVAL);
    Serial.print(".");
    break;

  case MODE_SVR_INIT:
    WiFi.mode(WIFI_AP);
    Serial.printf("WiFi.softAP(%s) .. ", SVR_SSID.c_str());
    if (! WiFi.softAP(SVR_SSID.c_str()) ) {
      Serial.println("failed");

      WiFi.mode(WIFI_OFF);

      changeMode(MODE_ON);
      break;
    }

    Serial.println("started");
    delay(300); // Important!

    WiFi.softAPConfig(AP_IP, AP_IP, AP_SUBNET);

    dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
    dnsServer.start(DNS_PORT, "*", AP_IP);
    Serial.printf("DNS server[%d] started\n", DNS_PORT);
    
    webServer.on("/", handle_top);
    webServer.on("/select_ssid", handle_select_ssid);
    webServer.on("/save_ssid", handle_save_ssid);
    webServer.on("/confirm_reboot", handle_confirm_reboot);
    webServer.on("/do_reboot", handle_do_reboot);
    webServer.onNotFound(handle_top);
    webServer.begin();
    Serial.printf("Web server[%d] started\n", WEB_SVR_PORT);

    changeMode(MODE_SVR);
    break;

  case MODE_SVR:
    /*
    if (cur_ms - modeStartMs > 60 * 1000) {

      changeMode(MODE_ON);
      break;
    }
    */

    //===== AP mode
    dnsServer.processNextRequest();
    webServer.handleClient();
    break;
    
  case MODE_ON:
    if (loopCount > 10) {
      Serial.println();
      
      confData.ssid = "ytnet_a1x";
      //confData.ssid_pw = "a1@ytnet";
      confData.save();

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);
      
      changeMode(MODE_INIT);
      break;
    }

    //===== main


    delay(1000);
    Serial.print(".");
    break;
    
  default:
    changeMode(MODE_INIT);

    delay(1000);
    break;
  }
}
