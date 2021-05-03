/*
   WIFI_MGR for ESP32
   by kghr labo 2020
*/

// include the library code:
#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include "SPIFFS.h"

#include "ConfigData.h"

#define LED_BUILTIN 2

IPAddress apIP(192,168,1,100);
WebServer webServer(80);
const char* WIFIMGR_ssid = "WIFIMGR_ESP32";
const char* WIFIMGR_pass = "xxxxxxxx";
DNSServer dnsServer;

// parameters setting
const String defaultSSID = "";
const String defaultPASSWD = "";
String ssid = defaultSSID;
String passwd = defaultPASSWD;

// scan SSID
#define SSIDLIMIT 30
String ssid_rssi_str[SSIDLIMIT];
String ssid_str[SSIDLIMIT];

// ConfigData
ConfigData confData;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // put your setup code here, to run once:

  confData.readConfigFile(CONFIG_FILE);
  confData.print();

  // wifi connect
  uint8_t retry = 0;
  WiFi.begin(confData.ssid.c_str(), confData.ssid_pw.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(200); // 200ms
    retry ++;
    if (retry > 50) { // 200ms x 50 = 10 sec
      Serial.println("wifi connection timeout");
      webconfig(); // enter webconfig
    }
  }
  Serial.println();
  Serial.printf("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:  
  
  delay(100);
}


//*******************************************
void webconfig() {
  
  Serial.println("WebConfig mode: ");
  digitalWrite(LED_BUILTIN, HIGH);

  configserver();
  
  uint8_t configloop = 1;
  while (configloop == 1) {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }

  digitalWrite(LED_BUILTIN, LOW);
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void configserver() {
  Serial.println("configserver>");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFIMGR_ssid); // no password
  //   WiFi.softAP(WIFIMGR_ssid,WIFIMGR_pass); // with password  
  Serial.println("configserver> " + String(WIFIMGR_ssid));

  delay(300); // Important! This delay is necessary 
  WiFi.softAPConfig(apIP,apIP,IPAddress(255,255,255,0)); 

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);
  Serial.println("DNS server: start");
  
  webServer.on("/", wifimgr_top);
  webServer.on("/wifiinput", HTTP_GET, wifiinput);
  webServer.on("/wifiset", HTTP_GET, wifiset);
  webServer.on("/reboot", reboot);
  webServer.on("/doreboot", doreboot);
  webServer.onNotFound(wifimgr_top);
  webServer.begin();
  Serial.println("webServer.begin()");
}

String maskpasswd(String passwd){
  String maskpasswd = "";

  for (int i=0; i<passwd.length(); i++) maskpasswd = maskpasswd + "*";
  if (passwd.length() == 0) maskpasswd = "(null)";

  return maskpasswd;
}

void wifimgr_top() {
  Serial.println("wifimgr_top> ");

  String html = Headder_str();
  html += "<a href='/wifiinput'>WIFI setup</a>";
  html += "<hr><h3>Current Settings</h3>";
  html += "SSID: " + ssid + "<br>";
  html += "passwd: " + maskpasswd(passwd) + "<br>";
  html += "<hr><p><center><a href='/reboot'>Reboot</a></center>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

String Headder_str() {
  Serial.println("Headder_str> ");
  
  String html = "";
  html += "<!DOCTYPE html><html><head>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.3'>";
  html += "<meta http-equiv='Pragma' content='no-cache'>";
  html += "<meta http-equiv='Cache-Control' content='no-cache'></head>";
  html += "<meta http-equiv='Expires' content='0'>";
  html += "<meta http-equiv='Content-type' CONTENT='text/html; charset=utf-8'>";
  html += "<style>";
  html += "a:link, a:visited { background-color: #009900; color: white; padding: 5px 15px;";
  html += "text-align: center; text-decoration: none;  display: inline-block;}";
  html += "a:hover, a:active { background-color: green;}";
  html += "bo32{ background-color: #EEEEEE;}";
  html += "input[type=button], input[type=submit], input[type=reset] {";
  html += "background-color: #000099;  border: none;  color: white;  padding: 5px 20px;";
  html += "text-decoration: none;  margin: 4px 2px;";
  html += "</style>";
  html += "<body>"; 
  html += "<h2>WIFI設定</h2>";
  return html;
}

void InitialConfigFile(){
  Serial.printf("SPIFFS initial file: %s\n", CONFIG_FILE);

  confData.ssid = defaultSSID;
  confData.ssid_pw = defaultPASSWD;
  
  //WriteConfigFile();
  confData.writeConfigFile(CONFIG_FILE);
}

void wifiinput() {
  String html = Headder_str();
  html += "<a href='/'>TOP</a> ";
  html += "<hr><p>";
  html += "<h3>WiFi Selector</h3>";
  html += WIFI_Form_str();
  html += "<br><hr><p><center><a href='/'>Cancel</a></center>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

//*******************************************
String WIFI_Form_str(){
  Serial.println("wifi scan start");

  // WiFi.scanNetworks will return the number of networks found
  uint8_t ssid_num = WiFi.scanNetworks();
  Serial.println("scan done\r\n");
  
  if (ssid_num == 0) {
    Serial.println("no networks found");
  } else {
    Serial.printf("%d networks found\r\n\r\n", ssid_num);
    if (ssid_num > SSIDLIMIT) ssid_num = SSIDLIMIT;
    for (int i = 0; i < ssid_num; ++i) {
      ssid_str[i] = WiFi.SSID(i);
      String wifi_auth_open = ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      ssid_rssi_str[i] = ssid_str[i] + " (" + WiFi.RSSI(i) + "dBm)" + wifi_auth_open;
      ssid_rssi_str[i] = ssid_str[i] + wifi_auth_open;
      Serial.printf("%d: %s\r\n", i, ssid_rssi_str[i].c_str());
      delay(10);
    }
  }
  
  String str = "";
  str += "<form action='/wifiset' method='get'>";
  str += "<select name='ssid' id ='ssid'>";
  for(int i=0; i<ssid_num; i++){
    str += "<option value=" + ssid_str[i] + ">" + ssid_rssi_str[i] + "</option>";
  }
  str += "<option value=" + ssid + ">" + ssid + "(current)</option>";
  if (ssid != defaultSSID){
    str += "<option value=" + defaultSSID + ">" + defaultSSID + "(default)</option>";
  }
  str += "</select><br>\r\n";
  str += "Password<br><input type='password' name='passwd' value='" + passwd + "'>";
  str += "<br><input type='submit' value='set'>";
  str += "</form><br>";
  str += "<script>document.getElementById('ssid').value = '"+ ssid +"';</script>";
  return str;
}


void wifiset(){
  Serial.println("wifiset>");

  confData.ssid = webServer.arg("ssid");
  confData.ssid_pw = webServer.arg("passwd");
  confData.ssid.trim();
  confData.ssid_pw.trim();
  confData.print();
 
  confData.writeConfigFile(CONFIG_FILE);
  
  // 「/」に転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}

void reboot() {
  String html = Headder_str();
  html += "<hr><p>";
  html += "<h3>reboot confirmation</h3><p>";
  html += "Are you sure to reboot?<p>";
  html += "<center><a href='/doreboot'>YES</a> <a href='/'>no</a></center>";
  html += "<p><hr>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

void doreboot() {
  String html = Headder_str();
  html += "<hr><p>";
  html += "<h3>rebooting</h3><p>";
  html += "The setting WiFi connection will be disconnected...<p>";
  html += "<hr>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);

  // reboot esp32
  Serial.println("reboot esp32 now.");
  digitalWrite(LED_BUILTIN, LOW);
    
  delay(2000); // hold 2 sec
  ESP.restart(); // restart ESP32
}
