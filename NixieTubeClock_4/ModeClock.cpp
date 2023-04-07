/**
 * Copyright (c) 2023 Yoichi Tanibayashi
 */
#include "ModeClock.h"
#include "NetMgr.h"

static const unsigned int CL_FADE_OFF = 0;
static const unsigned int CL_FADE_IN  = 1;
static const unsigned int CL_FADE_OUT = 2;
int colon_fade_mode[NIXIE_COLON_N] = {CL_FADE_OFF, CL_FADE_OFF};

static const unsigned long CL_FADE_TICK0 = 20;
static const unsigned long CL_FADE_TICK1 = 120;
static unsigned long cFadeTick = CL_FADE_TICK0;

static DateTime prev_dt = DateTime(2000,1,1,0,0,0);

#include <Adafruit_NeoPixel.h>
extern Adafruit_NeoPixel   Pixels;
static const uint8_t       PIXEL_N = 6;
static const int           PIXEL_BL = 255;
static const unsigned long PixelCol[] =
  {0x000000, 0xffffff, 0x0000ff, 0x0000ff};
static boolean pixel_on = false;
static boolean prev_pixel_on = pixel_on;

extern Button* Btn[];
extern NetMgr netMgr;
extern boolean wifiActive;
extern boolean prev_wifiActive;
extern boolean ntpActive;

/**
 *
 */
ModeClock::ModeClock(NixieArray *nxa): ModeBase::ModeBase(nxa,
                                                          "Clock",
                                                          ModeClock::TICK_MS) {
} // ModeClock::ModeClock()

/**
 *
 */
void ModeClock::init(unsigned long start_ms, DateTime& now,
                     int init_val[NIXIE_NUM_N]) {
  ModeBase::init(start_ms, now, init_val);
  this->mode_start_ms = millis();
  log_i("ModeClock::init> mode_start_ms=%ld, stat=0x%X",
        this->mode_start_ms, (int)this->stat);

  Nx->brightness = this->brightness;
}

/**
 *
 */
stat_t ModeClock::loop(unsigned long cur_ms, DateTime& now) {
  char disp_str[6 + 1];
  int  prev_num[NIXIE_NUM_N];
  static unsigned long prev_mode = MODE_NULL;

  if ( ModeBase::loop(cur_ms, now) == STAT_SKIP ) {
    return STAT_SKIP;
  }

  if ( this->stat != STAT_NONE && this->stat != STAT_DONE ) {
    log_i("stat=0x%X", (int)this->stat);
    return this->stat;
  }

  if ( pixel_on != prev_pixel_on ) {
    log_i("pixel_on=%d", pixel_on);
    prev_pixel_on = pixel_on;

    if ( pixel_on ) {
      for (int i=0; i < PIXEL_N; i++) {
        Pixels.setPixelColor(i, PixelCol[this->mode]);
      } // for(i)
    } else {
      Pixels.clear();
    } // if (pixel_on)
    Pixels.show();
    prev_mode = this->mode;
  } // if
  
  if ( pixel_on ) {
    if ( this->mode != prev_mode ) {
      for (int i=0; i < PIXEL_N; i++) {
        Pixels.setPixelColor(i, PixelCol[this->mode]);
      } // for(i)
      Pixels.show();
      prev_mode = this->mode;
    } // if
  } //if
  
  // disp_str に各桁の数字を格納
  switch ( this->mode ) {
  case ModeClock::MODE_HMS:
    sprintf(disp_str, "%02d%02d%02d", now.hour(), now.minute(), now.second());
    break;

  case ModeClock::MODE_DHM:
    sprintf(disp_str, "%02d%02d%02d", now.day(), now.hour(), now.minute());
    break;

  case ModeClock::MODE_YMD:
    if ( cur_ms - this->mode_start_ms > DISP_DATE_MS ) {
      this->change_mode(MODE_HMS);

      this->stat = STAT_DONE;
      return this->stat;
    }

    sprintf(disp_str, "%02d%02d%02d",
            now.year() % 100, now.month(), now.day());
    break;

  case ModeClock::MODE_MDH:
    if ( cur_ms - this->mode_start_ms > DISP_DATE_MS ) {
      this->change_mode(MODE_HMS);

      this->stat = STAT_DONE;
      return this->stat;
    }
    sprintf(disp_str, "%02d%02d%02d",
            now.month(), now.day(), now.hour());
    break;

  default:
    sprintf(disp_str, "%02d%02d%02d",
            now.hour(), now.minute(), now.second());
    break;
  } // switch (mode)

  // disp_str の内容をニキシー管に反映
  for (int i=0; i < NIXIE_NUM_N; i++) {
    prev_num[i] = this->_num[i];

    this->_num[i] = int(disp_str[i] - '0');
    if ( this->_num[i] != prev_num[i] ) {
      NxNum(i).xfade_start(cur_ms, FADE_TICK_MS,
                           this->_num[i], prev_num[i]);
    }
  } // for(NUM)

  // コロンの表示
  for (int i=0; i < NIXIE_COLON_N; i++) {
    if ( prev_dt.second() != now.second() ) {
      NxColEl(i, NIXIE_COLON_DOT_DOWN).set_brightness(Nx->brightness);

      //if ( this->mode != ModeClock::MODE_MDH || wifiActive ) {
      if ( wifiActive ) {
        NxCol(i).fadeout_start(cur_ms, cFadeTick,
                               NIXIE_COLON_DOT_DOWN);
        colon_fade_mode[i] = CL_FADE_OUT;
        continue;
      }
    }

    if (NxCol(i).effect_is_active()) {
      continue;
    }

    // effect is inactive
    if(colon_fade_mode[i] == CL_FADE_OUT) {
      NxCol(i).fadein_start(cur_ms, cFadeTick, NIXIE_COLON_DOT_DOWN);
      colon_fade_mode[i] = CL_FADE_IN;
    }
  } // for(COLON)

  prev_dt = DateTime(now);

  this->stat = STAT_DONE;
  return this->stat;
} // ModeClock::loop()

void ModeClock::change_mode(unsigned long mode=ModeClock::MODE_NULL) {
  log_i("mode=%ld", this->mode);

  if ( mode != MODE_NULL ) {
    this->mode = mode;
  } else {
    switch ( this->mode ) {
    case MODE_HMS:
      this->mode = MODE_MDH;
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
  this->mode_start_ms = millis();
  log_i("mode=%ld, mode_start_ms=%ld", this->mode, this->mode_start_ms);
}

void ModeClock::btn_intr_hdr(unsigned long cur_ms, Button *btn) {
} // ModeClock::btn_intr_hdr()

void ModeClock::btn_loop_hdr(unsigned long cur_ms, Button *btn) {
  boolean      flag = false;
  unsigned int bl = Nx->brightness;
  
  if ( btn->get_name() == "BTN0" ) {
    // BTN0
    if ( btn->is_long_pressed() && ! btn->is_repeated() ) {
      this->stat = ModeBase::STAT_NEXT_MODE;
      log_i("stat=0x%X", (int)this->stat);
      return;
    }

    if ( btn->get_click_count() == 1 ) {
      if ( Btn[1]->get_value() == Button::OFF ) {
        this->change_mode(); // 日付/時刻 切り替え
        return;
      }

      // BTN1:ON && BTN0:clicked
      // WiFi ON/OFF
      log_i("netMgr.cur_mode=0x%02X", netMgr.cur_mode);

      wifiActive = false;
      prev_wifiActive = false;
      ntpActive = false;

      if ( netMgr.cur_mode == NetMgr::MODE_AP_LOOP ||
           netMgr.cur_mode == NetMgr::MODE_WIFI_OFF ) {
        //netMgr.cur_mode = NetMgr::MODE_START;
        log_i("Reboot");
        log_i("==========");
        log_i("");
        ESP.restart();
      } else {
        netMgr.cur_mode = NetMgr::MODE_AP_INIT;
      }
      log_i("netMgr.cur_mode=0x%02X", netMgr.cur_mode);
      delay(500);
      return;

    }
    return;
  }

  if ( btn->get_name() == "BTN1" ) {
    // BTN1
    return;
  }

  // BTN2
  if ( btn->get_click_count() == 1 ) {
    if ( Btn[1]->get_value() == Button::ON ) {
        // ハードの最高輝度に設定
        bl = BRIGHTNESS_RESOLUTION;
    } else {
      bl /= 2;
      if (bl < BRIGHTNESS_MIN || bl > BRIGHTNESS_MAX) {
        bl = BRIGHTNESS_MAX;
      }
    }
  
    Nx->brightness = bl;
    this->brightness = bl;
    log_i("Nx->brightness=%d", Nx->brightness);

    for (int i=0; i < NIXIE_NUM_N; i++) {
      NxNumEl(i, this->_num[i]).set_brightness(bl);
    } // for(NUM)
  
    for (int i=0; i < NIXIE_COLON_N; i++) {
      NxColEl(i, NIXIE_COLON_DOT_DOWN).set_brightness(bl);
    } // for(COLON)

    return;
  } // if ( btn->get_click_count() >= 1)

  if ( btn->get_click_count() >= 2 ) {
    pixel_on = !pixel_on;
    log_i("pixel_on=%d", pixel_on);
  } 
  
} // ModeClock::btn_loop_hdr()
