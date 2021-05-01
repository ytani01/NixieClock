/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeClock2.h"

extern NixieArray *nixieArray;

ModeClock2::ModeClock2(NixieArray *nxa): ModeBase::ModeBase(nxa,
                                                            "ModeClock2",
                                                            ModeClock2::TICK_MS) {

  this->_rtc.begin();
  Serial.println("RTC start");
}

void ModeClock2::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  for (int i=0; i < NIXIE_NUM_N; i++) {
    this->_num[i] = i;
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      if ( this->_num[i] == e ) {
        this->_nxa->num[i].element[e].set_blightness(this->_nxa->blightness);
      } else {
        this->_nxa->num[i].element[e].set_blightness(0);
      }
    } // for(e)
  } // for(i)
}

void ModeClock2::loop(unsigned long cur_ms) {
  const char* WDAY[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
  char time_str[6 + 1];
  char dt_str[ModeClock2::DT_STR_LEN];
  DateTime now = this->_rtc.now();
  int prev_num[NIXIE_NUM_N];
  // int ch_flag[NIXIE_NUM_N];

  
  if ( cur_ms != 0 ) {
    if ( ! this->tick(cur_ms) ) {
      return;
    }
  }

  sprintf(time_str, "%02d%02d%02d", now.hour(), now.minute(), now.second());
  for (int i=0; i < NIXIE_NUM_N; i++) {
    prev_num[i] = this->_num[i];
    this->_num[i] = int(time_str[i] - '0');
    if ( this->_num[i] != prev_num[i] ) {
      this->_nxa->num[i].xfade_start(cur_ms, ModeClock2::FADE_TICK_MS,
                                     this->_num[i], prev_num[i]);
    }
  } // for(NUM)

  for (int i=0; i < NIXIE_COLON_N; i++) {
    if ( this->_num[5] != prev_num[5] ) {
      this->_nxa->colon[i].fadeout_start(cur_ms, 40,
                                         NIXIE_COLON_DOT_DOWN);
    } else {
      if ( this->_nxa->colon[i].effect_is_active() ) {
        // Serial.println("FADE active!");
      } else {
        this->_nxa->colon[i].element[0].set_blightness(this->_nxa->blightness);
      }
    }
  } // for(COLON)

  sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
          now.year(), now.month(), now.day(), WDAY[now.dayOfTheWeek()],
          now.hour(), now.minute(), now.second());

  char msg[1024];
  sprintf(msg, "ModeClock2::loop>[%d] dt_str(RTC)=%s, %s",
          int(this->_tick), dt_str, time_str);

  // Serial.println(msg);

} // ModeClock2::loop()

void ModeClock2::btn_intr(unsigned long cur_ms, Button *btn) {
  String msg = "ModeClock2::btn_intr>";
  msg += " " + String(btn->get_name());
  msg += " " + String(btn->get_click_count());
  Serial.println(msg);

  boolean flag = false;

  if ( btn->get_name() == "BTN1" && btn->get_click_count() > 0 ) {
    nixieArray->blightness *= 2 * btn->get_click_count() * btn->get_click_count();
    if (nixieArray->blightness > BLIGHTNESS_RESOLUTION) {
      nixieArray->blightness = BLIGHTNESS_RESOLUTION;
    }
    flag = true;
  }
  if ( btn->get_name() == "BTN2" && btn->get_click_count() > 0 ) {
    nixieArray->blightness /= 2 * btn->get_click_count() * btn->get_click_count();
    if (nixieArray->blightness < 1) {
      nixieArray->blightness = 1;
    }
    flag = true;
  }
  if (flag) { // 
    Serial.println("ModeClock2::nixieArray->blightness="
                   + String(nixieArray->blightness));
    for (int i=0; i < NIXIE_NUM_N; i++) {
      for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
        if (this->_nxa->num[i].element[e].get_blightness() > 0) {
          this->_nxa->num[i].element[e].set_blightness(nixieArray->blightness);
        }
      } // for(e)
    } // for(NUM)

    for (int i=0; i < NIXIE_COLON_N; i++) {
      for (int e=0; e < NIXIE_COLON_DOT_N; e++) {
        if (this->_nxa->colon[i].element[e].get_blightness() > 0) {
          this->_nxa->colon[i].element[e].set_blightness(nixieArray->blightness);
        }
      } // for(e)
    } // for(COLON)
  }
} // ModeClock2::btn_intr()

// for emacs
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
