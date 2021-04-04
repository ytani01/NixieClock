/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "Nixie.h"
#include "Button.h"
#include "ModeBase.h"
#include "ModeTest1.h"
#include "ModeTest2.h"
#include "ModeClock1.h"

#define LOOP_DELAY_US       1 // micro seconds
#define DEBOUNCE          200 // msec

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
/* WiFi */
const char* SSID = "fablabkannai";
const char* SSID_PW = "kannai201";

//============================================================================
/* for NTP */
struct tm timeInfo;

//============================================================================
/* RTC DS3231 */
RTC_DS3231 Rtc;
char dayOfTheWeek[7][4] =
  {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

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
#define MODE_NONE -1
#define MODE_TEST1 0
#define MODE_TEST2 1
#define MODE_CLOCK1 2
ModeTest1 *modeT1;
ModeTest2 *modeT2;
ModeClock1 *modeC1;

ModeBase *Mode[] = {modeT1, modeT2, modeC1};

static unsigned long MODE_N = sizeof(Mode) / sizeof(ModeBase *);

long curMode = MODE_TEST1;
long prevMode = MODE_NONE;
//============================================================================
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
  Serial.println("init> sec=" + String(sec));
  randomSeed(sec);

  nixieArray = new NixieArray(PIN_HV5812_CLK,  PIN_HV5812_STOBE,
                              PIN_HV5812_DATA, PIN_HV5812_BLANK,
                              nixiePins, colonPins);
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 各モードの初期化
  Mode[0] = new ModeTest1(nixieArray);
  Mode[1] = new ModeTest2(nixieArray);
  Mode[2] = new ModeClock1(nixieArray);
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
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("waiting WiFi: " + String(SSID));
  }
  Serial.println("Connected WiFi: " + String(SSID));

  //--------------------------------------------------------------------------
  configTime(9 * 3600L, 0, "ntp.nict.jp", "time.google.com");
  getLocalTime(&timeInfo);
  DateTime now = DateTime(timeInfo.tm_year + 1900,
                          timeInfo.tm_mon + 1,
                          timeInfo.tm_mday,
                          timeInfo.tm_hour,
                          timeInfo.tm_min,
                          timeInfo.tm_sec);
  Rtc.adjust(now);

  char dt_str[128];
  sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
          timeInfo.tm_year + 1900, timeInfo.tm_mon + 1, timeInfo.tm_mday,
          dayOfTheWeek[timeInfo.tm_wday],
          timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
  Serial.println("setup> dt_str(NTP)=" + String(dt_str));
} // setup()

//============================================================================
void loop() {
  char dt_str[128];

  prevMsec = curMsec;
  curMsec = millis();
  loopCount++;

  //--------------------------------------------------------------------------
  if (loopCount % 20000 == 0) {
    getLocalTime(&timeInfo);
    sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
            timeInfo.tm_year + 1900,
            timeInfo.tm_mon + 1,
            timeInfo.tm_mday,
            dayOfTheWeek[timeInfo.tm_wday],
            timeInfo.tm_hour,
            timeInfo.tm_min,
            timeInfo.tm_sec);
    Serial.println();
    Serial.println("dt_str(NTP)=" + String(dt_str));
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
