/**
 * (c) 2021 Yoichi Tanibayashi
 */
#include "netMgr.h"

static WebServer web_svr(NetMgr::WEBSVR_PORT);
String NetMgr::myName = "NetMgr";

static SSIDent ssidEnt[NetMgr::SSID_N_MAX];
static unsigned int ssidN;

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
    Serial.printf("%s> |%s|%s|\n",
                  myname.c_str(), ssid.c_str(), ssid_pw.c_str());

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
    uint8_t mac_int[6];
    char    mac_str[13];
    
    Serial.println(myname + "> MODE_AP_INIT");

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

    NetMgr::async_scan_ssid_start();

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
  html += ".myname {";
  html += "  font-size: xx-large;";
  html += "  text-align: center;";
  html += "  color: #FF0000;";
  html += "  background-color: #444444;";
  html += "}";
  html += ".ssid {";
  html += " font-size: x-large;";
  html += "}";
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
  html += " background-color: #00DD00;";
  html += "}";
  html += "input[type=button], input[type=submit], input[type=reset] {";
  html += " background-color: #0000DD;";
  html += " color: white;";
  html += " text-decoration: none;";
  html += " font-size: large;";
  html += " border: none;";
  html += " padding: 5px 20px;";
  html += " margin: 4px 2px;";
  html += "}";
  html += "</style>";

  html += "</head>";
  html += "<body style='background: linear-gradient(to left, #8EE, #4CC);'>";
  html += "<br />";
  html += "<div class='myname'>";
  html += NetMgr::myName;
  html += "</div>";
  html += "<div style='font-size:x-large; color:#00F; background-color: #DDD;'>";
  html += title;
  html += "</div>";
  html += "<hr>";
  return html;
} // NetMgr::handle_header()

String NetMgr::html_footer() {
  String html = "";
  html += "</body>";
  html += "</html>\n";
  return html;
} // NetMgr::html_footer();

void NetMgr::async_scan_ssid_start() {
  Serial.println("NetMgr::async_scan_ssid_start> ..");
  WiFi.scanNetworks(true);
}

unsigned int NetMgr::async_scan_ssid_wait(SSIDent ssid_ent[]) {
  int16_t ret;

  Serial.print("NetMgr::async_scan_ssid_wait> ");
  while ( (ret = WiFi.scanComplete()) == WIFI_SCAN_RUNNING ) {
    Serial.print(".");
    delay(200);
  }
  Serial.println();
  if ( ret == WIFI_SCAN_FAILED ) {
    return 0;
  }

  if ( ret > NetMgr::SSID_N_MAX ) {
    ret = NetMgr::SSID_N_MAX;
  }

  for (int i=0; i < ret; i++) {
    ssid_ent[i].set(WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i));
  } // for()

  return ret;
}

/**
 *
 */
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
void NetMgr::handle_top() {
  String myname = "NetMgr::handle_top";
  ConfigData conf_data;
  String ssid, ssid_pw;

  conf_data.load();
  ssid = conf_data.ssid;
  ssid_pw = conf_data.ssid_pw;
  
  Serial.printf("%s> ssid=%s, ssid_pw=%s\n",
                myname.c_str(), ssid.c_str(), ssid_pw.c_str());

  String html = NetMgr::html_header("WiFi Setting");
  html += "<span style='font-size: large;'>";
  html += "SSID: ";
  html += "</span>";
  html += "<span style='font-size: x-large; font-weight: bold'>";
  html += ssid;
  html += "</span>";
  html += "<hr />";
  html += "<a href='/select_ssid'>Change</a>\n";
  html += "or\n";
  html += "<a href='/confirm_reboot'>OK (Reboot clock)</a>\n";
  html += NetMgr::html_footer();
  web_svr.send(200, "text/html", html);
}

void NetMgr::handle_select_ssid() {
  ConfigData conf_data;
  String ssid, ssid_pw;
  

  conf_data.load();
  ssid = conf_data.ssid;
  ssid_pw = conf_data.ssid_pw;
  
  //ssidN = NetMgr::scan_ssid(ssidEnt);
  ssidN = NetMgr::async_scan_ssid_wait(ssidEnt);
  Serial.println("ssidN=" + String(ssidN));

  for (int i=0; i < ssidN; i++) {
    Serial.print(ssidEnt[i].ssid());
    Serial.print(" ");
    Serial.print(String(ssidEnt[i].dbm()));
    Serial.print(" ");
    Serial.print(ssidEnt[i].encType());
    Serial.println();
  } // for(i)

  String html = NetMgr::html_header("Please, select SSID");
  html += "<form action='/save_ssid' method='GET'>";
  html += "<div class='ssid'>";
  html += "SSID ";
  html += "<select name='ssid' id='ssid' style='font-size:large;'>";

  for(int i=0; i < ssidN; i++){
    html += "<option value=" + ssidEnt[i].ssid();
    if ( ssidEnt[i].ssid() == ssid ) {
      html += " selected";
    }
    html += ">";
    html += ssidEnt[i].ssid();
    /*
    html += " (";
    html += String(ssidEnt[i].dbm());
    html += ", ";
    html += ssidEnt[i].encType();
    html += ")";
    */
    html += "</option>\n";
  } // for(i)

  html += "<option value="">(clear)</option>\n";

  html += "</select><br />\n";
  html += "Password ";
  html += "<span style='font-size: xx-large'>";
  html += "<input type='password'";
  html += " name='passwd'";
  html += " value='" + ssid_pw + "'";
  html += " />";
  html += "</span>";
  html += "</div>\n";
  html += "<hr />\n";
  html += "<input type='submit' value='Save' />\n";
  html += "or\n";
  html += "<a href='/'>Cancel</a>\n";
  html += "</form>";
  html += NetMgr::html_footer();
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
  String html = NetMgr::html_header("Reboot confirmation");
  html += "<p>Are you sure to reboot?</p>\n";
  html += "<a href='/do_reboot'>Yes</a>";
  html += " or ";
  html += "<a href='/'>No</a>";
  html += NetMgr::html_footer();
  web_svr.send(200, "text/html", html.c_str());
}

void NetMgr::handle_do_reboot() {
  String html = NetMgr::html_header("Rebooting ..");
  html += "Please reconnect WiFi ..";
  html += "<hr />";
  html += NetMgr::html_footer();
  web_svr.send(200, "text/html", html.c_str());

  Serial.println("reboot esp32 ..\n");
    
  delay(2000);
  ESP.restart();
}
