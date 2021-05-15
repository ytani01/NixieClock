/*
 * (c) 2021 Yoichi Tanibayashi
 */
#include "Nixie.h"
#include "Button.h"
#include "NetMgr.h"
#include "ModeBase.h"
#include "ModeClock2.h"
#include "ModeTest1.h"
#include "ModeTest2.h"

static const String MY_NAME = "Nixie Tube Clock";

#define LOOP_DELAY_US       2 // micro sbeconds
#define DEBOUNCE          300 // msec
String dayOfWeekStr[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//======================================================================
// pin definitions
//----------------------------------------------------------------------
#define PIN_HV5812_CLK     26
#define PIN_HV5812_STOBE   13
#define PIN_HV5812_DATA    14
#define PIN_HV5812_BLANK    4

#define PIN_COLON_R_TOP    16
#define PIN_COLON_R_BOTTOM 16
#define PIN_COLON_L_TOP    17
#define PIN_COLON_L_BOTTOM 17

#define PIN_LED            27

#define PIN_BTN0           33
#define PIN_BTN1           34
#define PIN_BTN2           35
#define BTN_N               3

//======================================================================
// NetMgr
//----------------------------------------------------------------------
NetMgr netMgr;
boolean wifiActive = false;
boolean prev_wifiActive = false;

//======================================================================
// NTP
//----------------------------------------------------------------------
const unsigned long ntpInterval = 1000 * 30; // msec
unsigned long       ntpLast = 0;
boolean             ntpActive = false;
const char*         ntpSvr[] = {"ntp.nict.jp", "time.google.com", ""};

//======================================================================
// RTC DS3231
//----------------------------------------------------------------------
RTC_DS3231 Rtc;

//======================================================================
uint8_t pinsIn[] = {PIN_BTN0, PIN_BTN1, PIN_BTN2};
//----------------------------------------------------------------------
uint8_t nixiePins[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N] =
  {{ 9,  0,  1,  2,  3,  4,  5,  6,  7,  8},
   {19, 10, 11, 12, 13, 14, 15, 16, 17, 18},
   {29, 20, 21, 22, 23, 24, 25, 26, 27, 28},
   {39, 30, 31, 32, 33, 34, 35, 36, 37, 38},
   {49, 40, 41, 42, 43, 44, 45, 46, 47, 48},
   {59, 50, 51, 52, 53, 54, 55, 56, 57, 58} };
//----------------------------------------------------------------------
uint8_t colonPins[NIXIE_COLON_N][NIXIE_COLON_DOT_N] =
  {{PIN_COLON_R_TOP},
   {PIN_COLON_L_TOP} };
//----------------------------------------------------------------------
NixieArray nixieArray(PIN_HV5812_CLK,  PIN_HV5812_STOBE,
                      PIN_HV5812_DATA, PIN_HV5812_BLANK,
                      nixiePins, colonPins);
Button *btnObj[3];
//----------------------------------------------------------------------
unsigned long loopCount  = 0;
unsigned long curMsec    = 0; // msec
unsigned long prevMsec   = 0;
//----------------------------------------------------------------------
ModeBase mmm[] = {
                  ModeClock2(&nixieArray),
                  ModeTest1(&nixieArray),
                  ModeTest2(&nixieArray)
};
const unsigned long MMM_N = sizeof(mmm) / sizeof(ModeBase);

#define MODE_N 3
ModeBase *Mode[MODE_N];
long curMode = 0;
long prevMode = -1;

//======================================================================
void ntp_adjust() {
  struct tm time_info;

  getLocalTime(&time_info); // NTP
  DateTime now = DateTime(time_info.tm_year + 1900,
                          time_info.tm_mon + 1,
                          time_info.tm_mday,
                          time_info.tm_hour,
                          time_info.tm_min,
                          time_info.tm_sec);
  Rtc.adjust(now);

  Serial.printf("ntp_adjust> %04d/%02d/%02d(%s) %02d:%02d:%02d\n",
                time_info.tm_year + 1900,
                time_info.tm_mon + 1,
                time_info.tm_mday,
                dayOfWeekStr[time_info.tm_wday].c_str(),
                time_info.tm_hour,
                time_info.tm_min,
                time_info.tm_sec);
} // ntp_adjust()

long change_mode() {
  nixieArray.end_all_effect();
  prevMode = curMode;
  curMode = (curMode + 1) % MODE_N;
  Serial.println("change_mode(): curMode=" + String(curMode));
  return curMode;
} // change_mode()

void btn_hdr() {
  static unsigned long prev_msec = 0;
  unsigned long cur_msec = millis();

  //Serial.println("btn_hdr>");
  
  if ( cur_msec - prev_msec < DEBOUNCE ) {
    return;
  }

  prev_msec = cur_msec;

  for (int b=0; b < BTN_N; b++) {
    if ( btnObj[b]->get() ) {
      if ( b == 0 && btnObj[b]->get_click_count() >= 2 ) {
	change_mode();
      }
      // btnObj[b]->print();
      Mode[curMode]->btn_hdr(curMsec, btnObj[b]);
    }
  } // for(b)
} // btn_hdr

//=======================================================================
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println("setup> begin");
  Serial.printf("MMM_N=%d\n", MMM_N);
  for (int i=0; i < MMM_N; i++) {
    Serial.printf("mmm[%d]:%s\n", i, mmm[i].name());
  }
  
  //---------------------------------------------------------------------
  // グローバルオブジェクト・変数の初期化
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  NetMgr::myName = MY_NAME;

  Serial.println("setup> RTC begin");
  Rtc.begin();
  unsigned long sec = Rtc.now().second();
  Serial.println("setup> sec=" + String(sec));
  randomSeed(sec);

  nixieArray.blightness = BLIGHTNESS_RESOLUTION;

  ntpActive = false;
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 各モードオブジェクト生成
  Mode[0] = new ModeClock2(&nixieArray);
  Mode[1] = new ModeTest1(&nixieArray);
  Mode[2] = new ModeTest2(&nixieArray);

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ボタンの初期化
  btnObj[0] = new Button(PIN_BTN0, "BTN0");
  btnObj[1] = new Button(PIN_BTN1, "BTN1");
  btnObj[2] = new Button(PIN_BTN2, "BTN2");
  //-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
  uint8_t intr_pin0 = digitalPinToInterrupt(PIN_BTN0);
  uint8_t intr_pin1 = digitalPinToInterrupt(PIN_BTN1);
  uint8_t intr_pin2 = digitalPinToInterrupt(PIN_BTN2);
  Serial.println("digitalPinToInterrupt:");
  Serial.println(" " + String(PIN_BTN0) + " --> " + String(intr_pin0));
  Serial.println(" " + String(PIN_BTN1) + " --> " + String(intr_pin1));
  Serial.println(" " + String(PIN_BTN2) + " --> " + String(intr_pin2));

  attachInterrupt(intr_pin0, btn_hdr, CHANGE);
  attachInterrupt(intr_pin1, btn_hdr, CHANGE);
  attachInterrupt(intr_pin2, btn_hdr, CHANGE);
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 時間
  prevMsec = millis();
  curMsec = millis();
  //---------------------------------------------------------------------
  nixieArray.display(curMsec); // 初期状態表示
} // setup()

//=======================================================================
void loop() {
  netmgr_mode_t netmgr_mode;
  DateTime now = Rtc.now();

  prevMsec = curMsec;
  curMsec = millis();
  loopCount++;

  //---------------------------------------------------------------------
  // NetMgr
  netmgr_mode = netMgr.loop();
  prev_wifiActive = wifiActive;
  if (netmgr_mode == NetMgr::MODE_WIFI_ON) {
    wifiActive = true;
    ntpActive = true;
    if ( wifiActive != prev_wifiActive ) {
      Serial.println("loop> WiFi ON");
      configTime(9 * 3600L, 0, ntpSvr[0], ntpSvr[1], ntpSvr[2]);
      ntp_adjust();
    }
  } else if ( netmgr_mode == NetMgr::MODE_WIFI_OFF ) {
    wifiActive = false;
    ntpActive = false;
    if ( wifiActive != prev_wifiActive ) {
      Serial.println("loop> WiFi OFF .. try reconnect");
      netMgr.cur_mode = NetMgr::MODE_START;
    }
  }
  
  //---------------------------------------------------------------------
  if ((curMsec - ntpLast) >= ntpInterval) {
    ntpLast = curMsec;
    if (ntpActive) {
      ntp_adjust();
    } else {
      Serial.println("loop> ntpActive=false");
    }
  }

  if (loopCount % 5000 == 0) {
    Serial.printf("loop> now=%04d/%02d/%02d(%s) %02d:%02d:%02d",
                  now.year(), now.month(), now.day(),
                  dayOfWeekStr[now.dayOfTheWeek()].c_str(),
                  now.hour(), now.minute(), now.second());
    Serial.printf(" blightness=%d", nixieArray.blightness);
    Serial.println();
  }

  //---------------------------------------------------------------------
  // モード実行
  if (curMode != prevMode) {
    // モード変更時の初期化
    Mode[curMode]->init(curMsec);
    prevMode = curMode;
  } else {
    Mode[curMode]->loop(curMsec, now);
  }

  //---------------------------------------------------------------------
  // check buttions
  for (int b=0; b < BTN_N; b++) {
    if ( ! btnObj[b]->get() ) {
      continue;
    }

    // button status was chenged
    btnObj[b]->print();
    if ( b != 0 ) {
      Mode[curMode]->btn_hdr(curMsec, btnObj[b]);
      continue;
    }

    // BTN0
    if ( btnObj[b]->get_click_count() >= 3 ) {
      change_mode();
      break;
    }

    if ( btnObj[b]->get_click_count() >= 2 ) {
      wifiActive = false;
      prev_wifiActive = false;
      if ( netMgr.cur_mode == NetMgr::MODE_AP_LOOP ) {
        netMgr.cur_mode = NetMgr::MODE_START;
      } else {
        netMgr.cur_mode = NetMgr::MODE_AP_INIT;
      }
      delay(500);
      break;
    }
  } // for(b)
  //---------------------------------------------------------------------
  // 表示
  nixieArray.display(curMsec);

  //---------------------------------------------------------------------
  //delayMicroseconds(LOOP_DELAY_US);
} // loop()

//=======================================================================
// Local Variables:
// Mode: c++-mode
// Coding: utf-8-unix
// End:
