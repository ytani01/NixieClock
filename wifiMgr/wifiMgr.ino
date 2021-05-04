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

IPAddress apIP(192,168,1,100);
WebServer webServer(80);
const char* WIFIMGR_ssid = "NIXIE_CLOCK1";
const char* WIFIMGR_pass = "xxxxxxxx";
DNSServer dnsServer;

// parameters setting
const String defaultSSID = "please_select_SSID";
const String defaultPASSWD = "default_pw";
String ssid = defaultSSID;
String passwd = defaultPASSWD;

#define SSID_N_MAX 30

// SSIDent
SSIDent ssidEnt[SSID_N_MAX];

NetSetup netSetup;

ConfigData confData;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);

  // put your setup code here, to run once:

  confData.readConfigFile();
  confData.print();

  ssid = confData.ssid;
  passwd = confData.ssid_pw;

  // wifi connect
  WiFi.begin(ssid.c_str(), passwd.c_str());
  for (int i=0; i < 10; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected.");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      delay(500);
      return;
    }

    Serial.print('.');
    delay(1000);
  } // for(i)

  Serial.println("timed out");

  Serial.println("WebConfig mode: ");
  digitalWrite(PIN_LED, HIGH);

  config_server();
  
  boolean configloop = true;
  while (configloop) {
    dnsServer.processNextRequest();
    webServer.handleClient();
    Serial.print(".");
    delay(100);
  }

  digitalWrite(PIN_LED, LOW);
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
} // setup()

void config_server() {
  Serial.println("config_server>");

  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  delay(100);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFIMGR_ssid); // no password
  //   WiFi.softAP(WIFIMGR_ssid,WIFIMGR_pass); // with password  
  Serial.println("config_server> " + String(WIFIMGR_ssid));

  delay(300); // Important! This delay is necessary 
  WiFi.softAPConfig(apIP, apIP, IPAddress(255,255,255,0)); 

  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", apIP);
  Serial.println("DNS server: start");
  
  webServer.on("/", handle_top);
  webServer.on("/select_ssid", HTTP_GET, select_ssid);
  webServer.on("/save_ssid", HTTP_GET, save_ssid);
  webServer.on("/confirm_reboot", confirm_reboot);
  webServer.on("/do_reboot", do_reboot);
  webServer.onNotFound(handle_top);

  webServer.begin();
  Serial.println("webServer.begin()");
}

String maskpasswd(String passwd){
  String maskpasswd = "";

  for (int i=0; i<passwd.length(); i++) {
    maskpasswd = maskpasswd + "*";
  }
  if (passwd.length() == 0) maskpasswd = "(null)";

  return maskpasswd;
}

void handle_top() {
  const char* myname = "handle_top";
  Serial.printf("%s> ssid=%s, passwd=%s\n",
                myname, ssid.c_str(), passwd.c_str());

  String html = HTML_header();
  html += "<hr>";
  html += "<h2>Current SSID: " + ssid + "</h2>";
  //html += "<h2>passwd: " + maskpasswd(passwd) + "</h2>";
  html += "<a href='/select_ssid'>Select WiFi SSID</a>";
  html += "<hr>";
  html += "<a href='/confirm_reboot'>Reboot</a>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

String HTML_header() {
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

void InitialConfigFile(){
  Serial.printf("SPIFFS initial file: %s\n", CONFIG_FILE);

  confData.ssid = defaultSSID;
  confData.ssid_pw = defaultPASSWD;

  confData.writeConfigFile(CONFIG_FILE);
}

void select_ssid() {
  String html = HTML_header();
  html += "<hr>";
  html += "<h3>Select WiFi SSID</h3>";
  html += WIFI_Form_str();
  html += "<hr>";
  html += "<a href='/'>Cancel</a>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

//======================================================================
String WIFI_Form_str(){
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
  if (ssid != defaultSSID){
    str += "<option value=" + defaultSSID + ">";
    str += defaultSSID + " (default)";
    str += "</option>\n";
  }
  str += "</select><br />\n";
  str += "<br />";
  str += "Password<br />";
  str += "<input type='password' name='passwd' value='" + passwd + "' />";
  str += "<br>";
  str += "<input type='submit' value='Save' />";
  str += "</form>";
  str += "<br />";
  str += "<script>document.getElementById('ssid').value = '"+ ssid +"';</script>";
  return str;
}


void save_ssid(){
  Serial.println("save_ssid>");

  confData.ssid = webServer.arg("ssid");
  confData.ssid.trim();
  confData.ssid_pw = webServer.arg("passwd");
  confData.ssid_pw.trim();
  confData.print();
 
  confData.writeConfigFile();

  ssid = confData.ssid;
  passwd = confData.ssid_pw;
  
  // 自動転送
  webServer.sendHeader("Location", String("/"), true);
  webServer.send(302, "text/plain", "");
}

void confirm_reboot() {
  String html = HTML_header();
  html += "<hr><p>";
  html += "<h3>reboot confirmation</h3><p>";
  html += "Are you sure to reboot?<p>";
  html += "<center><a href='/do_reboot'>YES</a> <a href='/'>no</a></center>";
  html += "<p><hr>";
  html += "</body></html>";
  webServer.send(200, "text/html", html);
}

void do_reboot() {
  String html = HTML_header();
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
void loop() {
  // put your main code here, to run repeatedly:  
  
  delay(100);
}
