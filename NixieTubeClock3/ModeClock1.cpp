/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeClock1.h"

ModeClock1::ModeClock1(NixieArray *nxa)
: ModeBase::ModeBase(nxa, "ModeClock1", ModeClock1::TICK_MS) {

  this->_rtc.begin();
  
  String msg = "ModeClock1::setup(): ";
  msg += "_name = " + this->_name;
  msg += ", ";
  msg += "_tick_ms = " + this->_tick_ms;
  Serial.println(msg);
}

void ModeClock1::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  for (int i=0; i < NIXIE_NUM_N; i++) {
    this->_num[i] = i;
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      if ( this->_num[i] == e ) {
        // this->_nxa->num[i].element[e].set_blightness(BLIGHTNESS_MAX);
      } else {
        this->_nxa->num[i].element[e].set_blightness(0);
      }
    } // for(e)
  } // for(i)
}

void ModeClock1::loop(unsigned long cur_ms) {
  const char* WDAY[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  char time_str[NIXIE_NUM_N + 1];
  char dt_str[ModeClock1::DT_STR_LEN];
  DateTime now = this->_rtc.now();
  int prev_num[NIXIE_NUM_N];
  // int ch_flag[NIXIE_NUM_N];

  if ( ! this->tick(cur_ms) ) {
    return;
  }

  sprintf(time_str, "%02d%02d%02d", now.hour(), now.minute(), now.second());
  for (int i=0; i < NIXIE_NUM_N; i++) {
    prev_num[i] = this->_num[i];
    this->_num[i] = int(time_str[i] - '0');
    if ( this->_num[i] == prev_num[i] ) {
      // this->_nxa->num[i].element[this->_num[i]].set_blightness(BLIGHTNESS_MAX);
    } else if (! this->_nxa->num[i].effect_is_active() ) {
      this->_nxa->num[i].xfade_start(cur_ms, ModeClock1::FADE_TICK_MS,
                                     this->_num[i], prev_num[i]);
      if ( this->_num[5] % 2 == 0 ) {
        this->_nxa->colon[NIXIE_COLON_L].fadein_start(cur_ms,
                                                      ModeClock1::FADE_TICK_MS,
                                                      NIXIE_COLON_DOT_DOWN);
        this->_nxa->colon[NIXIE_COLON_R].fadeout_start(cur_ms,
                                                       ModeClock1::FADE_TICK_MS,
                                                       NIXIE_COLON_DOT_DOWN);
      } else {
        this->_nxa->colon[NIXIE_COLON_L].fadeout_start(cur_ms,
                                                       ModeClock1::FADE_TICK_MS,
                                                       NIXIE_COLON_DOT_DOWN);
        this->_nxa->colon[NIXIE_COLON_R].fadein_start(cur_ms,
                                                      ModeClock1::FADE_TICK_MS,
                                                      NIXIE_COLON_DOT_DOWN);
      }
    }
  } // for(NUM)

  sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
          now.year(), now.month(), now.day(), WDAY[now.dayOfTheWeek()],
          now.hour(), now.minute(), now.second());
  Serial.println("ModeClock1::loop> dt_str(RTC)="
                 + String(dt_str)
                 + ", "
                 + String(time_str));

} // ModeClock1::loop()

void ModeClock1::btn_intr(unsigned long cur_ms, Button *btn) {
  Serial.println("ModeClock1::btn_intr(" + btn->get_name() + ")");

} // ModeClock1::btn_intr()

// for emacs
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
