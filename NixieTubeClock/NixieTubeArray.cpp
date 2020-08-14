/*
 * NixieTubeArray.cpp
 */
#include "NixieTubeArray.h"

//============================================================================
void NixieElement::init(uint8_t pin) {
  this->set_blightness(BLIGHTNESS_MAX);
  this->set_pin(pin);
  this->_cur_on = false;
}

void NixieElement::set_blightness(uint8_t blightness) {
  this->_blightness = blightness;
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

void NixieElement::set_onoff(uint8_t timing) {
  this->_cur_on = false;
  if ( this->_blightness > timing ) {
    this->_cur_on = true;
  }
}
boolean NixieElement::get_cur_on() {
  return this->_cur_on;
}
//============================================================================
NixieTubeDigit *NixieTube::get_nx_digit() {
  return this->_nx_digit;
}

void NixieTube::set_onoff(uint8_t timing) {
  for (int d=0; d < NIXIE_TUBE_DIGIT_N; d++) {
    this->_nx_digit[d].set_onoff(timing);
  } // for(d)
}
//============================================================================
void NixieTubeArray::init(uint8_t clk, uint8_t stobe,
                          uint8_t data, uint8_t blank,
                          uint8_t nixie[NIXIE_TUBE_N][NIXIE_TUBE_DIGIT_N],
                          uint8_t colon[NIXIE_COLON_N]) {
  this->_pin_clk = clk;
  this->_pin_stobe = stobe;
  this->_pin_data = data;
  this->_pin_blank = blank;

  for (int t=0; t < NIXIE_TUBE_N; t++) {
    for (int d=0; d < NIXIE_TUBE_DIGIT_N; d++) {
      (this->_nx_tube[t].get_nx_digit())[d].init(nixie[t][d]);
    } // for(d)
  } // for(t)

  for (int c=0; c < NIXIE_COLON_N; c++) {
    this->_nx_colon[c].init(colon[c]);
  } // for(c)
}

NixieTube *NixieTubeArray::get_nx_tube() {
  return _nx_tube;
}

NixieColon *NixieTubeArray::get_nx_colon() {
  return _nx_colon;
}

void NixieTubeArray::set_tube_blightness(uint8_t tube_i, uint8_t digit_i,
                                         uint8_t blightness) {
  (this->_nx_tube[tube_i].get_nx_digit())[digit_i].set_blightness(blightness);
}

void NixieTubeArray::set_colon_blightness(uint8_t colon_i,
                                          uint8_t blightness) {
  this->_nx_colon[colon_i].set_blightness(blightness);
}

void NixieTubeArray::set_onoff(uint8_t timing) {
  for (int t=0; t < NIXIE_TUBE_N; t++) {
    this->_nx_tube[t].set_onoff(timing);
  } // for(t)
  for (int c=0; c < NIXIE_COLON_N; c++) {
    this->_nx_colon[c].set_onoff(timing);
  } // for(c)
}

void NixieTubeArray::display(unsigned long cur_ms) {
  uint8_t timing = cur_ms % BLIGHTNESS_MAX;
  uint8_t nx_pin_n = NIXIE_TUBE_N * NIXIE_TUBE_DIGIT_N;
  uint8_t nx_val[nx_pin_n];

  this->set_onoff(timing);

  //--------------------------------------------------------------------------
  for (int p=0; p < nx_pin_n; p++) {
    nx_val[p] = HIGH;
  }

  for (int t=0; t < NIXIE_TUBE_N; t++) {
    for (int d=0; d < NIXIE_TUBE_DIGIT_N; d++) {
      NixieTubeDigit nd = (this->_nx_tube[t].get_nx_digit())[d];
      uint8_t pin = nd.get_pin();

      if ( nd.get_cur_on() ) {
        nx_val[pin] = LOW;
      }
    } // for(d)
  } // for(t)

  for (int p=(nx_pin_n - 1); p >=0; p--) {
    digitalWrite(_pin_data, nx_val[p]);
    digitalWrite(_pin_clk, HIGH);
    //delay(1);
    digitalWrite(_pin_clk, LOW);
    //delay(1);
  }
  digitalWrite(_pin_stobe, HIGH);
  //delay(1);
  digitalWrite(_pin_stobe, LOW);
  //delay(1);
  //--------------------------------------------------------------------------
  for (int c=0; c < NIXIE_COLON_N; c++) {
    uint8_t pin = this->_nx_colon[c].get_pin();
    if ( this->_nx_colon[c].get_cur_on() ) {
      digitalWrite(pin, HIGH);
    } else {
      digitalWrite(pin, LOW);
    }
  }
  //--------------------------------------------------------------------------
}
//============================================================================
