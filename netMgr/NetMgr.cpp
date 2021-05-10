/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "netMgr.h"

WebServer web_svr(NetMgr::WEBSVR_PORT);

NetMgr::NetMgr() {
  this->ap_ip      = IPAddress(this->ap_ip_int[0],
                               this->ap_ip_int[1],
                               this->ap_ip_int[2],
                               this->ap_ip_int[3]);
  this->ap_netmask = IPAddress(this->ap_netmask_int[0],
                               this->ap_netmask_int[1],
                               this->ap_netmask_int[2],
                               this->ap_netmask_int[3]);
}

netmgr_mode_t NetMgr::loop() {
  String myname = "NetMgr::loop";
  ConfigData conf_data;
  String ssid, ssid_pw;
  
  this->_loop_count++;

  switch (this->cur_mode) {
  case MODE_NULL:
    break;
    
  case MODE_START:
    Serial.println(myname + "> MODE_START");

    conf_data.load();
    ssid = conf_data.ssid;
    ssid_pw = conf_data.ssid_pw;
    Serial.printf("%s> |%s|%s|\n", myname.c_str(), ssid.c_str(), ssid_pw.c_str());

    WiFi.begin(ssid.c_str(), ssid_pw.c_str());
    delay(100);
    this->cur_mode = MODE_TRY_WIFI;
    this->_loop_count = 0;
    break;

  case MODE_TRY_WIFI:
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println(myname + "> ");
      Serial.println(WiFi.localIP());

      this->net_is_available = true;
      this->cur_mode = MODE_WIFI_ON;
    }

    if (this->_loop_count > WIFI_TRY_COUNT_MAX) {
      Serial.println(" WiFi faild");

      WiFi.disconnect(true);
      WiFi.mode(WIFI_OFF);

      this->cur_mode = MODE_AP_INIT;
      break;
    }

    Serial.print(String(this->_loop_count) + " ");
    delay(WIFI_TRY_INTERVAL);
    break;

  case MODE_AP_INIT:
    Serial.println(myname + "> MODE_AP_INIT");

    uint8_t mac_int[6];
    char    mac_str[13];
    
    esp_read_mac(mac_int, ESP_MAC_WIFI_STA);
    sprintf(mac_str, "%02X%02X%02X%02X%02X%02X",
            mac_int[0], mac_int[1], mac_int[2],
            mac_int[3], mac_int[4], mac_int[5]);
    Serial.printf("%s> MacAddr=%s\n", myname.c_str(), mac_str);

    this->ap_ssid = this->ap_ssid_hdr + String(mac_str);
    Serial.println(myname + "> ap_ssid=" + this->ap_ssid);

    WiFi.mode(WIFI_AP);
    Serial.printf("WiFi.softAP(%s) .. ", this->ap_ssid.c_str());
    if ( ! WiFi.softAP(this->ap_ssid.c_str()) ) {
      Serial.println("failed");
      WiFi.mode(WIFI_OFF);

      this->cur_mode = MODE_WIFI_OFF;
      break;
    }

    Serial.println("start");
    delay(300);

    WiFi.softAPConfig(this->ap_ip, this->ap_ip, this->ap_netmask);

    this->dns_svr.setErrorReplyCode(DNSReplyCode::NoError);
    this->dns_svr.start(DNS_PORT, "*", this->ap_ip);
    Serial.printf("%s> DNS server[%d] started\n", myname.c_str(), DNS_PORT);

    web_svr.on("/", NetMgr::handle_top);
    web_svr.on("/select_ssid", NetMgr::handle_select_ssid);
    web_svr.on("/save_ssid", NetMgr::handle_save_ssid);
    web_svr.on("/confirm_reboot", NetMgr::handle_confirm_reboot);
    web_svr.on("/do_reboot", NetMgr::handle_do_reboot);
    web_svr.onNotFound(NetMgr::handle_top);
    web_svr.begin();
    Serial.printf("%s> Web server[%d] started\n", myname.c_str(), WEBSVR_PORT);

    this->cur_mode = MODE_AP_LOOP;
    break;

  case MODE_AP_LOOP:
    this->dns_svr.processNextRequest();
    web_svr.handleClient();
    break;

  case MODE_WIFI_ON:
    if (WiFi.status() != WL_CONNECTED) {
      this->cur_mode = MODE_WIFI_OFF;
    }
    break;

  case MODE_WIFI_OFF:
    if (WiFi.status() == WL_CONNECTED) {
      this->cur_mode = MODE_WIFI_ON;
    }
    break;

  default:
    Serial.println(myname + "> unknown mode ???");
    delay(1000);
    break;
  } // switch

  return this->cur_mode;
}

/**
 *
 */
void NetMgr::handle_top() {
  String myname = "NetMgr::handle_top";
  ConfigData conf_data;
  String ssid, ssid_pw;

  conf_data.load();
  ssid = conf_data.ssid;
  ssid_pw = conf_data.ssid_pw;
  
  Serial.printf("%s> ssid=%s, ssid_pw=%s\n",
                myname.c_str(), ssid.c_str(), ssid_pw.c_str());

  String html = html_header("WiFi Setup");
  html += "<hr>";
  html += "<h2>Current SSID: " + ssid + "</h2>";
  html += "<a href='/select_ssid'>Select WiFi SSID</a>";
  html += "<hr />";
  html += "<a href='/confirm_reboot'>Reboot</a>";
  html += "</body></html>";
  web_svr.send(200, "text/html", html);
}

unsigned int NetMgr::scan_ssid(SSIDent ssid_ent[]) {
  Serial.println("NetMgr::scan_ssid> scan start ..");

  int ssid_n = WiFi.scanNetworks();

  Serial.printf("NetMgr::scan_ssid> %d SSIDs found\n", ssid_n);

  if (ssid_n <= 0) {
    return 0;
  }

  if ( ssid_n > NetMgr::SSID_N_MAX ) {
    ssid_n = NetMgr::SSID_N_MAX;
  }

  for (int i=0; i < ssid_n; i++) {
    ssid_ent[i].set(WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
  } // for()

  return ssid_n;
} // NetMgr::ssid_scan()

/**
 *
 */
String NetMgr::html_header(String title) {
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

void NetMgr::handle_select_ssid() {
  ConfigData conf_data;
  String ssid, ssid_pw;
  SSIDent ssid_ent[SSID_N_MAX];
  

  conf_data.load();
  ssid = conf_data.ssid;
  ssid_pw = conf_data.ssid_pw;
  
  uint8_t ssid_n = NetMgr::scan_ssid(ssid_ent);
  Serial.println(ssid_n);

  for (int i=0; i < ssid_n; i++) {
    Serial.print(ssid_ent[i].ssid());
    Serial.print(" ");
    Serial.print(String(ssid_ent[i].dbm()));
    Serial.print(" ");
    Serial.print(ssid_ent[i].encType());
    Serial.println();
  } // for(i)

  String html = html_header("Please, select SSID");
  html += "<hr>";

  html += "<form action='/save_ssid' method='GET'>";

  html += "SSID<br />";
  html += "<select name='ssid' id='ssid'>";

  for(int i=0; i < ssid_n; i++){
    html += "<option value=" + ssid_ent[i].ssid();
    if ( ssid_ent[i].ssid() == ssid ) {
      html += " selected";
    }
    html += ">";
    html += ssid_ent[i].ssid();
    html += " (";
    html += String(ssid_ent[i].dbm());
    html += ", ";
    html += ssid_ent[i].encType();
    html += ")";
    html += "</option>\n";
  } // for(i)

  html += "<option value="">(clear)</option>\n";

  html += "</select>\n";
  html += "<br />";
  html += "<br />";
  html += "Password";
  html += "<br />";
  html += "<input type='password' name='passwd' value='" + ssid_pw + "' />";
  html += "<br>";
  html += "<input type='submit' value='Save' />";
  html += "</form>";
  html += "<br />";
  // html += "<script>document.getElementById('ssid').value = '"+ ssid +"';</script>";

  html += "<hr>";
  //html += "<a href='/confirm_reboot'>Reboot</a>";
  html += "<a href='/'>Cancel</a>";
  html += "</body></html>";
  web_svr.send(200, "text/html", html);
}

void NetMgr::handle_save_ssid(){
  ConfigData conf_data;
  String ssid = web_svr.arg("ssid");
  String ssid_pw = web_svr.arg("passwd");
  
  Serial.printf("save_ssid> |%s|%s|\n", ssid.c_str(), ssid_pw.c_str());

  conf_data.ssid = ssid;
  conf_data.ssid.trim();
  conf_data.ssid_pw = ssid_pw;
  conf_data.ssid_pw.trim();
  conf_data.print();
 
  conf_data.save();

  // 自動転送
  web_svr.sendHeader("Location", String("/"), true);
  web_svr.send(302, "text/plain", "");
}

void NetMgr::handle_confirm_reboot() {
  String html = html_header("Reboot confirmation");
  html += "<hr />";
  html += "<p>Are you sure to reboot?</p>\n";
  html += "<a href='/do_reboot'>Yes</a>";
  html += " or ";
  html += "<a href='/'>No</a>";
  html += "<hr />";
  html += "</body></html>";
  web_svr.send(200, "text/html", html.c_str());
}

void NetMgr::handle_do_reboot() {
  String html = html_header("Rebooting ..");
  html += "<hr>";
  html += "<p>The setting WiFi connection will be disconnected...</p>";
  html += "<hr>";
  html += "</body></html>";
  web_svr.send(200, "text/html", html.c_str());

  Serial.println("reboot esp32 ..\n");
  //digitalWrite(PIN_LED, LOW);
    
  delay(2000);
  ESP.restart();
}
