/**
 * (c) Yoichi Tanibayashi
 */
#include "ModeSetClock.h"

/**
 *
 */
ModeSetClock::ModeSetClock(NixieArray *nxa): ModeBase::ModeBase(nxa,
                                                                "SetClock",
                                                                ModeSetClock::TICK_MS) {
} // ModeSetClock::ModeSetClock()

/**
 *
 */
unsigned long ModeSetClock::change_mode(unsigned long mode=ModeSetClock::MODE_NULL) {
  Serial.print("ModeSetClock::change_mode> mode=");

  if ( mode != MODE_NULL ) {
    this->_mode = mode;
    Serial.printf("0x%02X\n", this->_mode);
    return this->_mode;
  }
  
  // mode < 0 .. cyclic
  switch ( this->_mode ) {
  case MODE_YEAR:
    this->_mode = MODE_MONTH;
    break;
  case MODE_MONTH:
    this->_mode = MODE_DAY;
    break;
  case MODE_DAY:
    this->_mode = MODE_HOUR;
    break;
  case MODE_HOUR:
    this->_mode = MODE_MINUTE;
    break;
  default:
    this->_mode = MODE_NULL;
    break;
  } // switch(mode)
  
  Serial.printf("0x%02X\n", this->_mode);
  return this->_mode;
}// ModeSetClock::change_mode()

/**
 *
 */
void ModeSetClock::init(unsigned long start_ms, DateTime& now,
                        int init_val[NIXIE_NUM_N]) {
  char disp_str[NIXIE_NUM_N+1];
  sprintf(disp_str, "%02d%02d%02d", now.year() % 100, now.month(), now.day());

  Serial.printf("ModeSetClock::init> disp_str='%s'\n", disp_str);

  for (int i=0; i < NIXIE_NUM_N; i++) {
    init_val[i] = int(disp_str[i] - '0');
  }
  ModeBase::init(start_ms, now, init_val);
  
  this->_mode = MODE_YEAR;
} // ModeSetClock::init()

/**
 *
 */
void ModeSetClock::loop(unsigned long cur_ms, DateTime& now) {
  if ( cur_ms != 0 ) {
    if ( ! this->tick(cur_ms) ) {
      return;
    }
  }

  int e1, e2;
  switch (this->_mode) {
  case MODE_YEAR:
  case MODE_HOUR:
    e1 = 0;
    e2 = 1;
    break;
    
  case MODE_MONTH:
  case MODE_MINUTE:
    e1 = 2;
    e2 = 3;
    break;
    
  case MODE_DAY:
    e1 = 4;
    e2 = 5;
    break;

  default:
    e1 = 4;
    e2 = 5;
    break;
  } // switch(this->_mode)

  char disp_str[6+1];

  if ( this->_mode & MODE_DATE ) {
    sprintf(disp_str, "%02d%02d%02d", now.year() % 100, now.month(), now.day());
  } else {
    sprintf(disp_str, "%02d%02d%02d", now.hour(), now.minute(), 0);
  }
  //Serial.println(String(disp_str));

  for (int i=0; i < NIXIE_NUM_N; i++) {
    this->_num[i] = int(disp_str[i] - '0');
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      if ( e == this->_num[i] ) {
        NxNumEl(i, e).set_blightness(Nx->blightness);
      } else {
        NxNumEl(i, e).set_blightness(0);
      }
    } // for(e)
  } // for(i)

  for (int i=0; i < NIXIE_COLON_N; i++) {
    NxColEl(i, NIXIE_COLON_DOT_DOWN).set_blightness(0);
  } // for(i)
} // ModeSetClock::loop()

/**
 *
 */
void ModeSetClock::btn_intr_hdr(unsigned long cur_ms, Button *btn) {
} // ModeSetClock::btn_intr_hdr()

/**
 *
 */
void ModeSetClock::btn_loop_hdr(unsigned long cur_ms, Button *btn) {
  if ( btn->get_name() == "BTN0" ) {
    if ( btn->get_count() == 1 ) {
      (void)this->change_mode();
    }
    return;
  }
  
} // ModeSetClock::btn_loop_hdr()
