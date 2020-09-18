/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "Nixie.h"
#include "Button.h"
#include "ModeBase.h"
#include "ModeTest1.h"

#define LOOP_DELAY_US       1 // micro seconds
#define DEBOUNCE          200 // msec

// #define PIN_LED            27

//============================================================================
NixieArray *nixieArray;
//----------------------------------------------------------------------------
#define PIN_HV5812_CLK     26
#define PIN_HV5812_STOBE   13
#define PIN_HV5812_DATA    14
#define PIN_HV5812_BLANK    4

uint8_t nixiePins[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N] =
  {{ 9,  0,  1,  2,  3,  4,  5,  6,  7,  8},
   {19, 10, 11, 12, 13, 14, 15, 16, 17, 18},
   {29, 20, 21, 22, 23, 24, 25, 26, 27, 28},
   {39, 30, 31, 32, 33, 34, 35, 36, 37, 38},
   {49, 40, 41, 42, 43, 44, 45, 46, 47, 48},
   {59, 50, 51, 52, 53, 54, 55, 56, 57, 58} };
//----------------------------------------------------------------------------
#define PIN_COLON_R_TOP    16
#define PIN_COLON_R_BOTTOM 16
#define PIN_COLON_L_TOP    17
#define PIN_COLON_L_BOTTOM 17

uint8_t colonPins[NIXIE_COLON_N][NIXIE_COLON_DOT_N] =
  {{PIN_COLON_R_TOP, PIN_COLON_R_BOTTOM},
   {PIN_COLON_L_TOP, PIN_COLON_L_BOTTOM} };
//----------------------------------------------------------------------------
#define PIN_BTN1           33
#define PIN_BTN2           34
#define PIN_BTN3           35
#define BTN_N               3

Button *btnObj[BTN_N];
//----------------------------------------------------------------------------
#define MODE_NONE  -1
#define MODE_TEST1  0
#define MODE_CLOCK  1
#define MODE_N     16

ModeBase *Mode[MODE_N];
int curMode  = MODE_TEST1;
int prevMode = MODE_NONE;
//----------------------------------------------------------------------------
unsigned long loopCount  = 0;
unsigned long curMsec    = 0; // msec
unsigned long prevMsec   = 0;
//============================================================================
void btn_handler() {
  static unsigned long prev_msec = 0;
  unsigned long cur_msec = millis();

  if ( cur_msec - prev_msec < DEBOUNCE ) {
    return;
  }
  prev_msec = cur_msec;

  for (int b=0; b < BTN_N; b++) {
    if ( btnObj[b]->get() ) {
      btnObj[b]->print(true);
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
  nixieArray = new NixieArray(PIN_HV5812_CLK,  PIN_HV5812_STOBE,
                              PIN_HV5812_DATA, PIN_HV5812_BLANK,
                              nixiePins, colonPins);
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 各モードの初期化
  Mode[MODE_TEST1] = new ModeTest1(nixieArray);
  // Mode[MODE_CLOCK] = new ModeClock(nixieyArray, rtc) ??
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ボタンの初期化
  btnObj[0] = new Button(PIN_BTN1, "BTN1");
  btnObj[1] = new Button(PIN_BTN2, "BTN2");
  btnObj[2] = new Button(PIN_BTN3, "BTN3");
  //-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -
  uint8_t intr_pin1 = digitalPinToInterrupt(PIN_BTN1);
  uint8_t intr_pin2 = digitalPinToInterrupt(PIN_BTN2);
  uint8_t intr_pin3 = digitalPinToInterrupt(PIN_BTN3);
  Serial.println("digitalPinToInterrupt:");
  Serial.println(" " + String(PIN_BTN1) + " --> " + String(intr_pin1));
  Serial.println(" " + String(PIN_BTN2) + " --> " + String(intr_pin2));
  Serial.println(" " + String(PIN_BTN3) + " --> " + String(intr_pin3));
  attachInterrupt(intr_pin1, btn_handler, CHANGE);
  attachInterrupt(intr_pin2, btn_handler, CHANGE);
  attachInterrupt(intr_pin3, btn_handler, CHANGE);
  //- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // 時間
  prevMsec = millis();
  curMsec = millis();
  //--------------------------------------------------------------------------
  nixieArray->display(curMsec); // 初期状態表示
} // setup()
//============================================================================
void loop() {
  prevMsec = curMsec;
  curMsec = millis();
  loopCount++;
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
      Mode[curMode]->btn_intr(curMsec, btnObj[b]);
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
