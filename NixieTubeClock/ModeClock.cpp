/*
 * (c) 2020 Yoichi Tanibayashi
 */
#include "ModeClock.h"

static const unsigned int C_FADE_OFF = 0;
static const unsigned int C_FADE_IN  = 1;
static const unsigned int C_FADE_OUT = 2;
int colon_fade_mode[NIXIE_COLON_N] = {C_FADE_OFF, C_FADE_OFF};

static const unsigned long C_FADE_TICK0 = 20;
static const unsigned long C_FADE_TICK1 = 120;
static unsigned long cFadeTick = C_FADE_TICK0;

extern boolean wifiActive;

/**
 *
 */
ModeClock::ModeClock(NixieArray *nxa): ModeBase::ModeBase(nxa,
                                                          "Clock",
                                                          ModeClock::TICK_MS) {
}

/**
 *
 */
void ModeClock::init(unsigned long start_ms) {
  ModeBase::init(start_ms);

  for (int i=0; i < NIXIE_NUM_N; i++) {
    this->_num[i] = i;
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      if ( this->_num[i] == e ) {
        NxNumEl(i, e).set_blightness(Nx->blightness);
      } else {
        NxNumEl(i, e).set_blightness(0);
      }
    } // for(e)
  } // for(i)
}

static DateTime prev_dt = DateTime(2000,1,1,0,0,0);

/**
 *
 */
void ModeClock::loop(unsigned long cur_ms, DateTime& now) {
  char disp_str[6 + 1];
  int  prev_num[NIXIE_NUM_N];

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

  switch ( this->mode ) {
  case ModeClock::MODE_HMS:
    sprintf(disp_str, "%02d%02d%02d", now.hour(), now.minute(), now.second());
    break;
  case MODE_DHM:
    sprintf(disp_str, "%02d%02d%02d", now.day(), now.hour(), now.minute());
    break;
  case ModeClock::MODE_YMD:
    sprintf(disp_str, "%02d%02d%02d",
            now.year() % 100, now.month(), now.day());
    break;
  default:
    sprintf(disp_str, "%02d%02d%02d", now.hour(), now.minute(), now.second());
    break;
  } // switch (mode)
  
  for (int i=0; i < NIXIE_NUM_N; i++) {
    prev_num[i] = this->_num[i];
    this->_num[i] = int(disp_str[i] - '0');
    if ( this->_num[i] != prev_num[i] ) {
      NxNum(i).xfade_start(cur_ms, ModeClock::FADE_TICK_MS,
                           this->_num[i], prev_num[i]);
    }
  } // for(NUM)

  for (int i=0; i < NIXIE_COLON_N; i++) {
    if ( prev_dt.second() != now.second() ) {
      NxColEl(i, NIXIE_COLON_DOT_DOWN).set_blightness(Nx->blightness);
      if ( wifiActive ) {
        NxCol(i).fadeout_start(cur_ms, cFadeTick,
                                         NIXIE_COLON_DOT_DOWN);
        colon_fade_mode[i] = C_FADE_OUT;
        continue;
      }
    }

    if (NxCol(i).effect_is_active()) {
      continue;
    }

    // effect is inactive
    if(colon_fade_mode[i] == C_FADE_OUT) {
      NxCol(i).fadein_start(cur_ms, cFadeTick, NIXIE_COLON_DOT_DOWN);
      colon_fade_mode[i] = C_FADE_IN;
    }
  } // for(COLON)
  prev_dt = DateTime(now);
} // ModeClock::loop()

void ModeClock::change_mode(unsigned long mode=ModeClock::MODE_NULL) {
  Serial.printf("change_mode(%d)> ", this->mode);

  if ( mode != MODE_NULL ) {
    this->mode = mode;
  } else {
    switch ( this->mode ) {
    case MODE_HMS:
      this->mode = MODE_DHM;
      break;
    case MODE_DHM:
      this->mode = MODE_YMD;
      break;
    case MODE_YMD:
      this->mode = MODE_HMS;
      break;
    default:
      this->mode = MODE_HMS;
      break;
    } // switch(mode)
  }
  Serial.printf("%d\n", this->mode);
}

void ModeClock::btn_hdr(unsigned long cur_ms, Button *btn) {
  boolean      flag = false;
  unsigned int bl = Nx->blightness;
  
  if ( btn->get_name() == "BTN1" ) {
    int count = btn->get_click_count();

    for (int i=0; i < count; i++) {
      this->change_mode();
    }
    return;
  }

  if ( btn->get_name() == "BTN1") {
    if ( btn->get_click_count() > 0 ) {
      bl *= 2 * btn->get_click_count();
    }
    if ( btn->is_repeated() ) {
      bl *= 2;
    }

    if (bl > BLIGHTNESS_RESOLUTION) {
      bl = 1;
    }
    flag = true;
  }
  if ( btn->get_name() == "BTN2" ) {
    if ( btn->get_click_count() > 0 ) {
      bl /= 2 * btn->get_click_count();
    }
    if ( btn->is_repeated() ) {
      bl /= 2;
    }
    
    if (bl < 1 || bl > BLIGHTNESS_RESOLUTION) {
      bl = BLIGHTNESS_RESOLUTION;
    }
    flag = true;
  }
  
  if (flag) {
    Nx->blightness = bl;
    for (int i=0; i < NIXIE_NUM_N; i++) {
      NxNumEl(i, this->_num[i]).set_blightness(bl);
    } // for(NUM)

    for (int i=0; i < NIXIE_COLON_N; i++) {
      NxColEl(i, NIXIE_COLON_DOT_DOWN).set_blightness(bl);
    } // for(COLON)
  }
} // ModeClock::btn_hdr()

// for emacs
// Local Variables:
// Mode: c++
// Coding: utf-8-unix
// End:
