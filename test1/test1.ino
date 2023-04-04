/**
 * Copyright (c) 2023 Yoichi Tanibayashi
 */
#include <Ticker.h>

#include "common.h"

#include "MainMode.h"

#include "ButtonTask.h"

#define MYNAME __FILE__

// ネット接続がなく、かつ、アイドル時間が経過したら、リセットする
const unsigned long IDEL_RESET = 5 * 60 * 1000; // 5 minutes
unsigned long idleStart = 0;

// Modes
#define curMode commonData.cur_mode

std::vector<ModeBase *> Mode;
MainMode *mainMode;

// common data
CommonData_t commonData;

// Display
Display_t *Disp;

// Buttons
constexpr uint8_t PIN_BTN_MODE = 33;
const String BTN_NAME_MODE = "ModeBtn";
ButtonWatcher *btnWatcher_Mode = NULL;
ButtonInfo_t btnInfo_Mode;

constexpr uint8_t PIN_BTN_SET = 34;
const String BTN_NAME_SET = "SetBtn";
ButtonWatcher *btnWatcher_Set = NULL;
ButtonInfo_t btnInfo_Set;

constexpr uint8_t PIN_BTN_BR = 35;
const String BTN_NAME_BR = "BrBtn";
ButtonWatcher *btnWatcher_Br = NULL;
ButtonInfo_t btnInfo_Br;

// Timer
constexpr TickType_t TIMER_INTERVAL = 2 * 1000; // tick == ms (?)
Ticker timer1;

/**
 *
 */
bool change_mode(Mode_t mode) {
  if ( ! Mode[curMode]->exit() ) {
    log_e("%s:exit(): failed", MODE_T_STR[curMode]);
    return false;
  }

  if ( ! Mode[mode]->enter(curMode) ) {
    log_e("%s:enter(): failed", MODE_T_STR[mode]);
  }

  Mode_t prev_mode = curMode;
  curMode = mode;
  log_i("mode: %s ==> %s",
        MODE_T_STR[prev_mode], MODE_T_STR[curMode]);
  return true;
} // change_mode()

/**
 *
 */
void do_restart() {
  log_w("restart..");
  
  commonData.msg = " Reboot.. ";
  delay(1000);
  
  //ESP.restart();
  ESP.deepSleep(100);
  delay(500);
} // do_restart()

/**
 * 【注意・重要】
 * コールバック実行中に、次のタイマー時間になると、
 * 次のコールバックが待たされ、あふれるとパニックする。
 */
void timer1_cb() {
  TickType_t tick1 = xTaskGetTickCount();
  log_d("[ ] timer test: start(priority=%d)",
        uxTaskPriorityGet(NULL));

  //  delay(TIMER_INTERVAL / 2);

  TickType_t tick2 = xTaskGetTickCount();
  TickType_t d_tick = tick2 - tick1;
  log_d("%d %d", tick1, tick2);
  log_d("[ ] timer test: end(d_tick=%d)", d_tick);
} // timer1_cb()

/** callback
 *
 */
void btnCb_Mode(ButtonInfo_t *btn_info) {
  idleStart = millis();

  log_i("%s", Button::info2String(btn_info).c_str());
  btnInfo_Mode = *btn_info;

  Mode_t dst_mode = Mode[curMode]->btnCb_Mode(btn_info);
  if ( dst_mode != MODE_N && dst_mode != curMode ) {
    change_mode(dst_mode);
  }
} // btnCb_Mode()

/**
 *
 */
void setup() {
  Serial.begin(115200);
  do {
    delay(100);
  } while (!Serial);  // Serial Init Wait
  Serial.println();
  Serial.println("===== start: " + String(MYNAME) + " =====");
  log_i("portTICK_PERIOD_MS=%d", portTICK_PERIOD_MS);

  // Tasks
  unsigned long task_interval = 10;

  btnWatcher_Mode = new ButtonWatcher(BTN_NAME_MODE, PIN_BTN_MODE, btnCb_Mode);
  btnWatcher_Mode->start();
  delay(task_interval);

  // start timer1
  timer1.attach_ms(TIMER_INTERVAL, timer1_cb);
  log_i("start Timer: %.1f sec", TIMER_INTERVAL / 1000.0);

  // init Mode[]
  mainMode = new MainMode("MainMode", &commonData);
  Mode.push_back(mainMode);

  for (int i=0; i < Mode.size(); i++) {
    log_i("%d:%s", i, Mode[i]->get_name().c_str());
    Mode[i]->setup();
  }
  change_mode(MODE_MAIN);

  idleStart = millis();
} // setup()

/**
 *
 */
void loop() {
  static unsigned long prev_ms = millis();
  unsigned long cur_ms = millis();
  int d_ms = cur_ms - prev_ms;
  prev_ms = cur_ms;

  /*
   * loop
   */
  Mode[curMode]->loop(cur_ms);

  /*
   * display
   */
  Mode[curMode]->display(Disp);

  // idle ms
  unsigned long idle_ms = millis() - idleStart;
  if ( idle_ms >= IDEL_RESET ) {
    log_i("idle_ms=%d .. reboot", idle_ms);
    //ESP.restart();
    ESP.deepSleep(100);
    delay(500);
  }
  
  delay(1);
} // loop()
