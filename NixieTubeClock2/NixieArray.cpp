/*
 * NixieArray.cpp
 */
#include "NixieArray.h"

//============================================================================
// class NixieElement
//----------------------------------------------------------------------------
void NixieElement::init(uint8_t pin) {
  //this->set_blightness(BLIGHTNESS_MAX);
  this->set_blightness(0);
  this->set_pin(pin);
  this->_on = false;
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

void NixieElement::onoff(uint8_t timing) {
  this->_on = false;
  if ( this->_blightness > timing ) {
    this->_on = true;
  }
}
boolean NixieElement::is_on() {
  return this->_on;
}
//============================================================================
// class NixieNum
//----------------------------------------------------------------------------
NixieNumDigit *NixieNum::get_digit() {
  return this->_digit;
}
NixieNumDigit *NixieNum::get_digit(uint8_t digit_i) {
  return &(this->_digit[digit_i]);
}

uint8_t NixieNum::get_blightness(uint8_t digit_i) {
  return this->_digit[digit_i].get_blightness();
}

void NixieNum::onoff(uint8_t timing) {
  for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
    this->_digit[d].onoff(timing);
  } // for(d)
}
//============================================================================
// class NixieArray
//----------------------------------------------------------------------------
void NixieArray::init(uint8_t clk, uint8_t stobe,
                          uint8_t data, uint8_t blank,
                          uint8_t num[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
                          uint8_t colon[NIXIE_COLON_N]) {
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
    for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
      (this->_num[n].get_digit())[d].init(num[n][d]);
    } // for(d)
  } // for(n)

  for (int c=0; c < NIXIE_COLON_N; c++) {
    pinMode(colon[c], OUTPUT);
    digitalWrite(colon[c], LOW);
    this->_colon[c].init(colon[c]);
  } // for(c)
}

NixieNum *NixieArray::get_num() { return _num; }
NixieNum *NixieArray::get_num(uint8_t num_i) { return &(_num[num_i]); }
NixieColon *NixieArray::get_colon() { return _colon; }

uint8_t NixieArray::get_num_blightness(uint8_t num_i, uint8_t digit_i) {
  return this->_num[num_i].get_blightness(digit_i);
}
void NixieArray::set_num_blightness(uint8_t num_i, uint8_t digit_i,
                                    uint8_t blightness) {
  (this->_num[num_i].get_digit())[digit_i].set_blightness(blightness);
}

void NixieArray::set_colon_blightness(uint8_t colon_i,
                                      uint8_t blightness) {
  this->_colon[colon_i].set_blightness(blightness);
}

void NixieArray::onoff(uint8_t timing) {
  for (int t=0; t < NIXIE_NUM_N; t++) {
    this->_num[t].onoff(timing);
  } // for(t)
  for (int c=0; c < NIXIE_COLON_N; c++) {
    this->_colon[c].onoff(timing);
  } // for(c)
}

void NixieArray::display(unsigned long cur_ms) {
  uint8_t timing = cur_ms % BLIGHTNESS_MAX;
  uint8_t pin_n = NIXIE_NUM_N * NIXIE_NUM_DIGIT_N;
  uint8_t val[pin_n];

  //Serial.println("cur_ms=" + String(cur_ms) + ", timing=" + String(timing));
  this->onoff(timing);

  //--------------------------------------------------------------------------
  for (int p=0; p < pin_n; p++) {
    val[p] = HIGH;
  }

  for (int t=0; t < NIXIE_NUM_N; t++) {
    for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
      NixieNumDigit nd = (this->_num[t].get_digit())[d];
      uint8_t pin = nd.get_pin();

      if ( nd.is_on() ) {
        val[pin] = LOW;
      }
    } // for(d)
  } // for(t)

  for (int p=(pin_n - 1); p >=0; p--) {
    //Serial.print(String(p) + ":" + String(val[p]) + " ");
    
    digitalWrite(_pin_data, val[p]);
    digitalWrite(_pin_clk, HIGH);
    //delay(1);
    digitalWrite(_pin_clk, LOW);
    //delay(1);
  }
  //Serial.println();
  
  digitalWrite(_pin_stobe, HIGH);
  //delay(1);
  digitalWrite(_pin_stobe, LOW);
  //delay(1);
  //--------------------------------------------------------------------------
  for (int c=0; c < NIXIE_COLON_N; c++) {
    uint8_t pin = this->_colon[c].get_pin();

    //Serial.print("pin[" + String(pin) + "]:");
    if ( this->_colon[c].is_on() ) {
      //Serial.println("on");
      digitalWrite(pin, HIGH);
    } else {
      //Serial.println("off");
      digitalWrite(pin, LOW);
    }
  }
  //--------------------------------------------------------------------------
}
//============================================================================
