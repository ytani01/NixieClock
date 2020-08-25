/*
 * (c) Yoichi Tanibayashi
 */
#include "NixieArray.h"

//============================================================================
// class NixieElement
//----------------------------------------------------------------------------
void NixieElement::setup(uint8_t pin) {
  this->set_pin(pin);

  this->set_blightness(0);
  this->_on = false;
}

void NixieElement::set_blightness(uint8_t blightness) {
  this->_blightness = blightness;
}
void NixieElement::set_blightness_zero() {
  this->_blightness = 0;
}
void NixieElement::set_blightness_max() {
  this->_blightness = BLIGHTNESS_MAX;
}
void NixieElement::inc_blightness() {
  if (this->_blightness < BLIGHTNESS_MAX) {
    this->_blightness++;
  }
}
void NixieElement::dec_blightness() {
  if (this->_blightness > 0) {
    this->_blightness--;
  }
}
uint8_t NixieElement::get_blightness() {
  return this->_blightness;
}

void NixieElement::set_pin(uint8_t pin) {
  this->_pin = pin;
}
uint8_t NixieElement::get_pin() {
  return this->_pin;
}

void NixieElement::on() {
  this->_on = true;
}
void NixieElement::off() {
  this->_on = false;
}
boolean NixieElement::is_on() {
  return this->_on;
}
//============================================================================
// class NixieTube
//----------------------------------------------------------------------------
void NixieTube::setup(int element_n, uint8_t *pin) {
  this->element_n = element_n;
  this->element = new NixieElement[this->element_n];
  for (int d=0; d < element_n; d++) {
    this->element[d].setup(pin[d]);
  } // for (d)
}

void NixieTube::loop(unsigned long cur_ms) {
  if (this->_effect == EFFECT_NONE) {
    return;
  }

  // XXX TBD
}
//============================================================================
// class NixieArray
//----------------------------------------------------------------------------
void NixieArray::setup(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
                       uint8_t num_pin[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
                       uint8_t colon_pin[NIXIE_COLON_N][NIXIE_COLON_DOT_N]) {
  uint8_t pout[] = {clk, stobe, data, blank};

  for (int p=0; p < sizeof(pout) / sizeof(uint8_t); p++) {
    pinMode(pout[p], OUTPUT);
    digitalWrite(pout[p], LOW);
  }
  this->_pin_clk   = clk;
  this->_pin_stobe = stobe;
  this->_pin_data  = data;
  this->_pin_blank = blank;

  for (int n=0; n < NIXIE_NUM_N; n++) {
    this->num[n].setup(NIXIE_NUM_DIGIT_N, num_pin[n]);
  } // for(n)

  for (int c=0; c < NIXIE_COLON_N; c++) {
    this->colon[c].setup(NIXIE_COLON_DOT_N, colon_pin[c]);
    for (int d=0; d < NIXIE_COLON_DOT_N; d++) {
      pinMode(colon_pin[c][d], OUTPUT);
      digitalWrite(colon_pin[c][d], LOW);
    }
  } // for(c)
} // NixieArray::setup()

void NixieArray::loop(unsigned long cur_ms) {
  for (int ti=0; ti < NIXIE_NUM_N; ti++) {
    this->num[ti].loop(cur_ms);
  } // for(ti)
  for (int ti=0; ti < NIXIE_COLON_N; ti++) {
    this->colon[ti].loop(cur_ms);
  } // for(ti)
}

void NixieArray::set_onoff(unsigned long cur_ms) {
  uint8_t timing = cur_ms % BLIGHTNESS_MAX;

  for (int t=0; t < NIXIE_NUM_N; t++) {
    for (int e=0; e < this->num[t].element_n; e++) {
      this->num[t].element[e].off();
      if (this->num[t].element[e].get_blightness() > timing) {
        this->num[t].element[e].on();
      }
    } // for(e)
  } // for(t)

  for (int t=0; t < NIXIE_COLON_N; t++) {
    for (int e=0; e < this->colon[t].element_n; e++) {
      this->num[t].element[e].off();
      if (this->num[t].element[e].get_blightness() > timing) {
        this->num[t].element[e].on();
      }
    } // for(e)
  } // for(t)
} // NixieArray::set_onoff()

void NixieArray::display(unsigned long cur_ms) {
  uint8_t pin_n = NIXIE_NUM_N * NIXIE_NUM_DIGIT_N;
  uint8_t val[pin_n];

  //--------------------------------------------------------------------------
  this->loop(cur_ms); // ニキシー管 全て
  //--------------------------------------------------------------------------
  this->set_onoff(cur_ms); // エレメント 全て
  //--------------------------------------------------------------------------
  // 数字部の表示処理
  for (int p=0; p < pin_n; p++) {
    val[p] = HIGH;
  }

  for (int t=0; t < NIXIE_NUM_N; t++) {
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      uint8_t pin = this->num[t].element[e].get_pin();

      if ( this->num[t].element[e].is_on() ) {
        val[pin] = LOW;
      }
    } // for(e)
  } // for(t)

  //? digitalWrite(_pin_clk, LOW);
  for (int p=(pin_n - 1); p >=0; p--) {
    digitalWrite(_pin_data, val[p]);
    digitalWrite(_pin_clk, HIGH);
    delayMicroseconds(NixieArray::DISP_DELAY_US);

    digitalWrite(_pin_clk, LOW);
    delayMicroseconds(NixieArray::DISP_DELAY_US);
  }
  
  digitalWrite(_pin_stobe, HIGH);
  delayMicroseconds(NixieArray::DISP_DELAY_US);

  digitalWrite(_pin_stobe, LOW);
  delayMicroseconds(NixieArray::DISP_DELAY_US);
  //--------------------------------------------------------------------------
  // コロンの表示処理
  for (int c=0; c < NIXIE_COLON_N; c++) {
    for (int d=0; d < NIXIE_COLON_DOT_N; d++) {
      uint8_t pin = this->colon[c].element[d].get_pin();

      //Serial.print("pin[" + String(pin) + "]:");
      if ( this->colon[c].element[d].is_on() ) {
        digitalWrite(pin, HIGH);
      } else {
        digitalWrite(pin, LOW);
      }
    } // for(d)
  } // for(c)
  //--------------------------------------------------------------------------
}
//============================================================================
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
