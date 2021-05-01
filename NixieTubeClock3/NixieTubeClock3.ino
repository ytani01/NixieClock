/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "Nixie.h"
#include "Button.h"
#include "ModeBase.h"
#include "ModeTest1.h"
#include "ModeTest2.h"
#include "ModeClock1.h"
#include "ModeClock2.h"

#define LOOP_DELAY_US       1 // micro seconds
#define WIFI_TRY_MAX       10 // count
#define DEBOUNCE          200 // msec

/*
const char* SSID = "fablabkannai";
const char* SSID_PW = "kannai201";
*/
const char* SSID = "ytnet_a1";
const char* SSID_PW = "a1@ytnet";
const char* ntpSvr[] = {"ntp.nict.jp", "time.google.com", ""};

//============================================================================
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

//============================================================================
/* for NTP */
const unsigned long ntpInterval = 1000 * 20; // msec
unsigned long       ntpLast = 0;
boolean             ntpActive = false;

//============================================================================
/* RTC DS3231 */
RTC_DS3231 Rtc;
char* dayOfTheWeek[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

//============================================================================
uint8_t pinsIn[] = {PIN_BTN0, PIN_BTN1, PIN_BTN2};
//----------------------------------------------------------------------------
uint8_t nixiePins[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N] =
  {{ 9,  0,  1,  2,  3,  4,  5,  6,  7,  8},
   {19, 10, 11, 12, 13, 14, 15, 16, 17, 18},
   {29, 20, 21, 22, 23, 24, 25, 26, 27, 28},
   {39, 30, 31, 32, 33, 34, 35, 36, 37, 38},
   {49, 40, 41, 42, 43, 44, 45, 46, 47, 48},
   {59, 50, 51, 52, 53, 54, 55, 56, 57, 58} };
//----------------------------------------------------------------------------
uint8_t colonPins[NIXIE_COLON_N][NIXIE_COLON_DOT_N] =
  {{PIN_COLON_R_TOP},
   {PIN_COLON_L_TOP} };
//----------------------------------------------------------------------------
NixieArray *nixieArray;
Button *btnObj[3];
//----------------------------------------------------------------------------
unsigned long loopCount  = 0;
unsigned long curMsec    = 0; // msec
unsigned long prevMsec   = 0;
//----------------------------------------------------------------------------
int curTube = 0;
int curDigit = 0;
//----------------------------------------------------------------------------
#define MODE_N 3
ModeBase *Mode[MODE_N];
long curMode = 0;
long prevMode = -1;

//============================================================================
void ntp_adjust() {
  struct tm time_info;

  getLocalTime(&time_info);
  DateTime now = DateTime(time_info.tm_year + 1900,
                          time_info.tm_mon + 1,
                          time_info.tm_mday,
                          time_info.tm_hour,
                          time_info.tm_min,
                          time_info.tm_sec);
  Rtc.adjust(now);

  char dt_str[128];
  sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
          time_info.tm_year + 1900, time_info.tm_mon + 1, time_info.tm_mday,
          dayOfTheWeek[time_info.tm_wday],
          time_info.tm_hour, time_info.tm_min, time_info.tm_sec);
  Serial.println("ntp_adjust> " + String(dt_str));
} // ntp_adjust()

long change_mode() {
  nixieArray->end_all_effect();
  prevMode = curMode;
  curMode = (curMode + 1) % MODE_N;
  Serial.println("change_mode(): curMode=" + String(curMode));
  return curMode;
} // change_mode()

void btn_handler() {
  static unsigned long prev_msec = 0;
  unsigned long cur_msec = millis();

  if ( cur_msec - prev_msec < DEBOUNCE ) {
    return;
  }
  prev_msec = cur_msec;

  for (int b=0; b < BTN_N; b++) {
    if ( btnObj[b]->get() ) {
      if ( b == 0 && btnObj[b]->get_click_count() >= 2 ) {
	change_mode();
      }
      btnObj[b]->print();
      Mode[curMode]->btn_intr(curMsec, btnObj[b]);
    }
  } // for(b)
} // btn_handler

//============================================================================
void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  Serial.println("begin");
  
  //--------------------------------------------------------------------------
  // グローバルオブジェクト・変数の初期化
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  WiFi.begin(SSID, SSID_PW);

  Rtc.begin();
  unsigned long sec = Rtc.now().second();
  Serial.println("setup> sec=" + String(sec));
  randomSeed(sec);

  nixieArray = new NixieArray(PIN_HV5812_CLK,  PIN_HV5812_STOBE,
                              PIN_HV5812_DATA, PIN_HV5812_BLANK,
                              nixiePins, colonPins);
  nixieArray->blightness = 10;
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 各モードオブジェクト生成
  //Mode[0] = new ModeClock1(nixieArray);
  Mode[0] = new ModeClock2(nixieArray);
  Mode[1] = new ModeTest1(nixieArray);
  Mode[3] = new ModeTest2(nixieArray);

  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ボタンの初期化
  btnObj[0] = new Button(PIN_BTN0, "BTN0");
  btnObj[1] = new Button(PIN_BTN1, "BTN1");
  btnObj[2] = new Button(PIN_BTN2, "BTN2");
  //-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
  uint8_t intr_pin0 = digitalPinToInterrupt(PIN_BTN0);
  uint8_t intr_pin1 = digitalPinToInterrupt(PIN_BTN1);
  uint8_t intr_pin2 = digitalPinToInterrupt(PIN_BTN2);
  Serial.println("digitalPinToInterrupt:");
  Serial.println(" " + String(PIN_BTN0) + " --> " + String(intr_pin0));
  Serial.println(" " + String(PIN_BTN1) + " --> " + String(intr_pin1));
  Serial.println(" " + String(PIN_BTN2) + " --> " + String(intr_pin2));

  attachInterrupt(intr_pin0, btn_handler, CHANGE);
  attachInterrupt(intr_pin1, btn_handler, CHANGE);
  attachInterrupt(intr_pin2, btn_handler, CHANGE);
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 時間
  prevMsec = millis();
  curMsec = millis();
  //--------------------------------------------------------------------------
  nixieArray->display(curMsec); // 初期状態表示
  //--------------------------------------------------------------------------

  //--------------------------------------------------------------------------
  // WIFI and NTP
  for (int i=0; i < WIFI_TRY_MAX; i++) {
    Serial.println("waiting WiFi: " + String(SSID));
    delay(500);
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected WiFi: " + String(SSID));
      Serial.println("IP addr: " + WiFi.localIP().toString());

      configTime(9 * 3600L, 0, ntpSvr[0], ntpSvr[1], ntpSvr[2]);
      ntpActive = true;
      ntp_adjust();
      break;
    }
  }
  if ( ! ntpActive ) {
    WiFi.mode(WIFI_OFF);
    Serial.println("setup> WiFi OFF");
  }

} // setup()

//============================================================================
void loop() {
  char dt_str[128];

  prevMsec = curMsec;
  curMsec = millis();
  loopCount++;

  //--------------------------------------------------------------------------
  if (ntpActive && (curMsec - ntpLast) >= ntpInterval) {
    ntp_adjust();
    ntpLast = curMsec;
  }

  if (loopCount % 2000 == 0) {
    DateTime now = Rtc.now();
    sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
            now.year(), now.month(), now.day(),
            dayOfTheWeek[now.dayOfTheWeek()],
            now.hour(), now.minute(), now.second());
    // Serial.println("dt_str(RTC)=" + String(dt_str));
  }

  //--------------------------------------------------------------------------
  // モード実行
  if (curMode != prevMode) {
    // モード変更時の初期化
    Mode[curMode]->init(curMsec);
    prevMode = curMode;
  } else {
    Mode[curMode]->loop(curMsec);
  }

  //--------------------------------------------------------------------------
  // ボタン
  for (int b=0; b < BTN_N; b++) {
    if ( btnObj[b]->get() ) {
      btnObj[b]->print();
      if ( btnObj[b]->get_name() == "BTN0" &&
           btnObj[b]->is_long_pressed() && ! btnObj[b]->is_repeated()) {
	change_mode();
      } else {
        Mode[curMode]->btn_intr(curMsec, btnObj[b]);
      }
    }
  }
  //--------------------------------------------------------------------------
  // 表示
  nixieArray->display(curMsec);
  //--------------------------------------------------------------------------
  delayMicroseconds(LOOP_DELAY_US);
} // loop()
//============================================================================
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
