/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeClock2.h"

static const unsigned int C_FADE_OFF = 0;
static const unsigned int C_FADE_IN  = 1;
static const unsigned int C_FADE_OUT = 2;
int colon_fade_mode[NIXIE_COLON_N] = {C_FADE_OFF, C_FADE_OFF};

static const unsigned long C_FADE_TICK0 = 35;
static const unsigned long C_FADE_TICK1 = 120;
static unsigned long cFadeTick = C_FADE_TICK0;

extern boolean wifiActive;

/**
 *
 */
ModeClock2::ModeClock2(NixieArray *nxa): ModeBase::ModeBase(nxa,
                                                            "ModeClock2",
                                                            ModeClock2::TICK_MS) {

  this->_rtc.begin();
  Serial.println("RTC start");
}

/**
 *
 */
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

/**
 *
 */
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

  if ( wifiActive ) {
    cFadeTick = C_FADE_TICK0;
  } else {
    cFadeTick = C_FADE_TICK1;
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
      this->_nxa->colon[i].fadeout_start(cur_ms, cFadeTick,
                                         NIXIE_COLON_DOT_DOWN);
      colon_fade_mode[i] = C_FADE_OUT;
      //Serial.println("ModeClock2::loop> " + String(i) + ":fadeout start");
      continue;
    }

    if (this->_nxa->colon[i].effect_is_active()) {
      continue;
    }

    // effect is inactive
    if(colon_fade_mode[i] == C_FADE_OUT) {
      this->_nxa->colon[i].fadein_start(cur_ms, cFadeTick,
                                        NIXIE_COLON_DOT_DOWN);
      colon_fade_mode[i] = C_FADE_IN;
      //Serial.println("ModeClock2::loop> " + String(i) + "fadein start");
    }
  } // for(COLON)

  sprintf(dt_str, "%04d/%02d/%02d(%s) %02d:%02d:%02d",
          now.year(), now.month(), now.day(), WDAY[now.dayOfTheWeek()],
          now.hour(), now.minute(), now.second());

} // ModeClock2::loop()

void ModeClock2::btn_intr(unsigned long cur_ms, Button *btn) {
  Serial.printf("ModeClock2::btn_intr> %s %d\n",
                btn->get_name().c_str(), btn->get_click_count());

  boolean flag = false;

  if ( btn->get_name() == "BTN1" && btn->get_click_count() > 0 ) {
    this->_nxa->blightness += btn->get_click_count();
    if (this->_nxa->blightness > BLIGHTNESS_RESOLUTION) {
      this->_nxa->blightness = BLIGHTNESS_RESOLUTION;
    }
    flag = true;
  }
  if ( btn->get_name() == "BTN2" && btn->get_click_count() > 0 ) {
    this->_nxa->blightness -= btn->get_click_count();
    if (this->_nxa->blightness < 1 || this->_nxa->blightness > BLIGHTNESS_RESOLUTION) {
      this->_nxa->blightness = 1;
    }
    flag = true;
  }
  
  if (flag) { // 
    Serial.println("ModeClock2::this->_nxa->blightness="
                   + String(this->_nxa->blightness));
    for (int i=0; i < NIXIE_NUM_N; i++) {
      this->_nxa->num[i].element[this->_num[i]].set_blightness(this->_nxa->blightness);
    } // for(NUM)

    for (int i=0; i < NIXIE_COLON_N; i++) {
      for (int e=0; e < NIXIE_COLON_DOT_N; e++) {
        this->_nxa->colon[i].element[e].set_blightness(this->_nxa->blightness);
      } // for(e)
    } // for(COLON)
  }
} // ModeClock2::btn_intr()

// for emacs
// Local Variables:
// Mode: c++
// Coding: utf-8-unix
// End:
