#include <WiFi.h>
#include "FS.h"
#include "SPIFFS.h"
#include "TimeLib.h" //Use Arduino time library ver1.5
#include "ESP32_WebGet.h" //mgo-tec library ver 1.12
 
const char *ap_ssid = "xxxxxxxxx"; //ESP32 softAP SSID
const char *ap_pass = "xxxxxxxxx"; //ESP32 softAP password
 
const char* AP_config_file = "/APconfig/APconfig.txt"; //ssid, password save file
 
IPAddress LIP; //Local IP address
WiFiServer server(80);
WiFiClient client;
 
ESP32_WebGet EWG;
 
uint8_t ssid_num;
String ssid_rssi_str[30];
String ssid_str[30];
String Selected_SSID_str = " ";
String Sel_SSID_PASS_str = " ";
 
uint32_t scanLastTime = 0;
boolean First_Scan_Set = true;
//*******************************************
void setup(){
  Serial.begin(115200);
  delay(500);
 
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS failed, or not present");
    return;
  }
 
  WiFi.mode(WIFI_AP_STA); //AP mode and STA mode
  WiFi.softAP(ap_ssid, ap_pass);
  delay(100);
 
  Serial.println("Setup done");
 
  server.begin();
  Serial.println(F("Server started"));
 
  scanLastTime = millis();
}
//*******************************************
void loop(){
  client_connect();
  wifi_scan(30000);
}
//*******************************************
void client_connect(){
  String html_res_head = "HTTP/1.1 200 OK\r\n";
         html_res_head += "Content-type:text/html\r\n";
         html_res_head += "Connection:close\r\n\r\n";
  String html_tag1 = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n";
         html_tag1 += "<meta name='viewport' content='initial-scale=1.5'>\r\n";
         html_tag1 += "</head>\r\n\r\n";
         html_tag1 += "<body style='background:#000; color:#fff; font-size:100%;'>\r\n";
         html_tag1 += "ESP32 (ESP-WROOM-32)<br>\r\n";
         html_tag1 += "Access Point Selector<br>\r\n";
  String html_tag2 = "\r\n</body>\r\n</html>\r\n\r\n";
 
  client = server.available();
 
  if (client) {
    Serial.println("new client");
    String req_str = "";
    while (client.connected()){
      while(client.available()){
        req_str =client.readStringUntil('\n');
        if(req_str.indexOf("\r") == 0) break;
        Serial.println(req_str);
        if(req_str.indexOf("GET / ") >= 0){
          Serial.println("--------------- GET Request Receive from Clinet");
          while(client.available()){
            char c = client.read();
            Serial.write(c);
          }
          Serial.println("--------------- GET Request Receive Finish");
          html_send(false, "ESP32 STA connection close", "ESP32 STA connection close", "#FFF", html_res_head, html_tag1, html_tag2);
 
          delay(10);
          client.stop();
          Serial.println("client disonnected");
          delay(10);
          req_str = "";
        }else if(req_str.indexOf("GET /?") >= 0){
          Serial.println("--------------- SUBMIT Receive from Clinet");
          int16_t getTXT_pass = req_str.indexOf("pass1=");
          int16_t getTXT_select = req_str.indexOf("ssid_select=");
          int16_t getTXT_close = req_str.indexOf("connection_close=");
 
          if(getTXT_pass > 0){
            Sel_SSID_PASS_str = req_str.substring(getTXT_pass + 6, req_str.indexOf("&ssid_sel_submit"));
          }
          if(getTXT_select > 0){
            Selected_SSID_str = req_str.substring(getTXT_select + 12, req_str.indexOf("&pass1"));
          }
          if(getTXT_close < 0){
            Serial.printf("Selected_SSID_str = %s\r\n", Selected_SSID_str.c_str());
            Serial.printf("Sel_SSID_PASS_str = %s\r\n", Sel_SSID_PASS_str.c_str());
 
            while(client.available()){
              char c = client.read();
              Serial.write(c);
            }
 
            Serial.println("-------------- SUBMIT Request Receive Finish");
            String str_w = Selected_SSID_str + "\r\n" + Sel_SSID_PASS_str;
 
            SPIFFS_writeFile(AP_config_file, str_w.c_str());
            delay(500);
 
            char spiffs_read_ssid[64] = {};
            char spiffs_read_pass[64] = {};
 
            SPIFFS_readFile(AP_config_file, spiffs_read_ssid, spiffs_read_pass);
            Serial.printf("SPIFFS read ssid = %s\r\n", (const char *)spiffs_read_ssid);
            Serial.printf("SPIFFS read pass = %s\r\n", (const char *)spiffs_read_pass);
            Serial.printf("\r\n%s Connecting ...\r\n", Selected_SSID_str.c_str());
 
            delay(10); // Important! This delay is necessary to connect to the Access Point.
            WiFi.begin(spiffs_read_ssid, spiffs_read_pass);
            uint32_t timeout = millis();
            while(1){
              if(WiFi.status() != WL_CONNECTED){
                delay(500);
                Serial.print(".");
                if(millis()-timeout > 20000){
                  html_send(false, "disconnected", "STA disconnected Time OUT", "#F00", html_res_head, html_tag1, html_tag2);
                  break;
                }
              }else{
                LIP = WiFi.localIP();
                Serial.println("\r\nWiFi connected");
                Serial.print("Local IP address: "); Serial.println(LIP);
                Serial.printf("\r\n-----------%s Connected!\r\n", spiffs_read_ssid);
 
                EWG.EWG_NTP_TimeLib_init(9, "time.windows.com"); //NTPサーバー取得初期化
                String NowTime = String(year()) + "/" + String(month()) + "/" + String(day()) + "/ " + String(hour()) + ":" + String(minute());
                html_send(true, Selected_SSID_str, "ESP32 STA connect OK!<br>"+ NowTime, "#0F0", html_res_head, html_tag1, html_tag2);
                Serial.printf("Now Time %s\r\n", NowTime.c_str());
                break;
              }
            }
          }else{
            html_send(false, "---", "ESP32 STA Closed!", "#F00", html_res_head, html_tag1, html_tag2);
            WiFi.disconnect(false); //false=WiFi_ON , true=WiFi_OFF
          }
 
          delay(10);
          client.stop();
          Serial.println("client disonnected");
          delay(10);
          req_str = "";
        }else if(req_str.indexOf("GET /favicon") >= 0){
          favicon_response();
          req_str = "";
        }
      }
    }
  }
}
//*******************************************
void html_send(boolean sta_connected, String message1, String message2, String color, String html_res_head, String html_tag1, String html_tag2){
  client.print(html_res_head);
  client.print(html_tag1);
  client.print(HTML_Select_Box_str("!xxxx", message1));
  client.printf("<p style='color:%s; font-size:80%%'>%s</p>\r\n", color.c_str(), message2.c_str());
  if(sta_connected == true){
    client.print("<span style='font-size:80%'>ESP32 STA IP = ");
    client.print(LIP);
    client.print("</span>\r\n");
  }
  client.print(html_tag2);
 
  Serial.print(html_res_head);
  Serial.print(html_tag1);
  Serial.print(HTML_Select_Box_str("!xxxx", message1));
  Serial.printf("<p style='color:%s; font-size:80%%'>%s</p>\r\n", color.c_str(), message2.c_str());
  if(sta_connected == true){
    Serial.print("<span style='font-size:80%%'>ESP32 STA IP = ");
    Serial.print(LIP);
    Serial.print("</span>\r\n");
  }
  Serial.print(html_tag2);
}
//*******************************************
String HTML_Select_Box_str(String button_id, String Sel_Ssid){
  String str = "";
  String selected_str = "";
  str += "<form name='F_ssid_select'>\r\n";
  str += "  <select name='ssid_select'>\r\n";
  for(int i=0; i<ssid_num; i++){
    if(Selected_SSID_str == ssid_str[i]){
      selected_str = " selected";
    }else{
      selected_str = "";
    }
    str += "    <option value=" + ssid_str[i] +  selected_str + ">" + ssid_rssi_str[i] + "</option>\r\n";
  }
  str += "</select><br>\r\n";
  str += "Password<br><input type='password' name='pass1'>\r\n";
  str += "<br><button type='submit' name='ssid_sel_submit' value='send' style='background-color:#AFA;' onclick='document.getElementById(\"ssid_sel_txt\").innerHTML=document.F_ssid_select.ssid_select.value;'>STA Connection GO!</button>\r\n";
  str += "</form><br>\r\n";
  str += "<form name='F_connection_close'>\r\n";
  str += "  <button type='submit' name='connection_close' value='send' style='background-color:#FAA;' onclick='document.getElementById(\"ssid_sel_txt\").innerHTML=\"ESP32 STA connection close\";'>STA Connection Close</button>\r\n";
  str += "</form>\r\n";
  str += "<br>  (Selected SSID)<br><span id='ssid_sel_txt'  style='font-size:80%;'>";
  str += Sel_Ssid;
  str += "</span>\r\n";
  return str;
}
//*******************************************
void wifi_scan(uint32_t scan_interval){
  if( (First_Scan_Set == true) || ((millis() - scanLastTime) > scan_interval) ){
    Serial.println("scan start");
 
    // WiFi.scanNetworks will return the number of networks found
    ssid_num = WiFi.scanNetworks();
    Serial.println("scan done\r\n");
    if (ssid_num == 0) {
      Serial.println("no networks found\r\n");
    } else {
      Serial.printf("%d networks found\r\n\r\n", ssid_num);
      for (int i = 0; i < ssid_num; ++i) {
        ssid_str[i] = WiFi.SSID(i);
        String wifi_auth_open = ((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        ssid_rssi_str[i] = ssid_str[i] + " (" + WiFi.RSSI(i) + "dBm)" + wifi_auth_open;
        Serial.printf("%d: %s\r\n", i, ssid_rssi_str[i].c_str());
        delay(10);
      }
    }
    Serial.println("");
    scanLastTime = millis();
    First_Scan_Set = false;
  }
}
//*******************************************
void SPIFFS_writeFile(const char * path, const char *c_ssid_pass){
    Serial.printf("SPIFFS writing file: %s\n", path);
    File file = SPIFFS.open(path, FILE_WRITE);
    if(!file){
      Serial.println("Failed to open file for writing");
      return;
    }
    file.seek(0);
    if(file.print(c_ssid_pass)){
      Serial.println(c_ssid_pass);
      Serial.println("SPIFFS file written\r\n");
    } else {
      Serial.println("SPIFFS write failed\r\n");
    }
    file.close();
}
//*******************************************
void SPIFFS_readFile(const char * path, char ssid_c[], char pass_c[]){
    Serial.printf("SPIFFS reading file: %s\n", path);
 
    File file = SPIFFS.open(path);
    if(!file || file.isDirectory()){
      Serial.println("SPIFFS Failed to open file for reading");
      return;
    }
 
    int i=0, j=0;
    while(file.available()){
      ssid_c[i] = file.read();
      if(ssid_c[i] == '\r'){
        ssid_c[i] = '\0';
        char c = file.read();
        if(c == '\n'){
          while(file.available()){
            pass_c[j++] = file.read();
          }
          break;
        }
      }
      i++;
    }
    if(i < 1) Serial.println("cannot read ssid");
    if(j < 1) Serial.println("cannot read password");
    file.close();
}
//*******************************************
void favicon_response(){
  Serial.println(F("-----------------------Favicon GET Request Received"));
  while(client.available()){
    Serial.write(client.read());
  }
 
  client.print(F("HTTP/1.1 404 Not Found\r\n"));
  client.print(F("Connection:close\r\n\r\n"));
 
  delay(10);
  client.stop();
  delay(10);
 
  Serial.println(F("-----------------Client.stop (by Favicon Request)"));
}
