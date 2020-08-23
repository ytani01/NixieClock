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
// class NixieTube
//----------------------------------------------------------------------------
void NixieTube::setup(int element_n, uint8_t *pin) {
  this->_element = new NixieElement[element_n];
  for (int d=0; d < element_n; d++) {
    this->_element[d].setup(pin[d]);
  } // for (d)
}

NixieElement *NixieTube::get_element() {
  return this->_element;
}
NixieElement *NixieTube::get_element(uint8_t element_i) {
  return &(this->_element[element_i]);
}

// 個別
void NixieTube::set_blightness(uint8_t element_i, uint8_t blightness) {
  this->_element[element_i].set_blightness(blightness);
}
void NixieTube::set_blightness_zero(uint8_t element_i) {
  this->_element[element_i].set_blightness_zero();
}
void NixieTube::set_blightness_max(uint8_t element_i) {
  this->_element[element_i].set_blightness_max();
}
uint8_t NixieTube::get_blightness(uint8_t element_i) {
  return this->_element[element_i].get_blightness();
}

// 一括
void NixieTube::set_blightness(uint8_t blightness[NIXIE_NUM_DIGIT_N]) {
  for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
    this->_element[d].set_blightness(blightness[d]);
  } // for(d)
}
void NixieTube::set_blightness_zero() {
  for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
    this->_element[d].set_blightness_zero();
  } // for(d)
}
void NixieTube::set_blightness_max() {
  for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
    this->_element[d].set_blightness_max();
  } // for(d)
}
void NixieTube::get_blightness(uint8_t blightness[NIXIE_NUM_DIGIT_N]) {
  for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
    blightness[d] = this->_element[d].get_blightness();
  } // for(d)
}

void NixieTube::onoff(uint8_t timing) {
  for (int d=0; d < NIXIE_NUM_DIGIT_N; d++) {
    this->_element[d].onoff(timing);
  } // for(d)
}
//============================================================================
// class NixieArray
//----------------------------------------------------------------------------
void NixieArray::setup(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
                       uint8_t num[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
                       uint8_t colon[NIXIE_COLON_N][NIXIE_COLON_DOT_N]) {
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
    this->_num[n].setup(NIXIE_NUM_DIGIT_N, num[n]);
  } // for(n)

  for (int c=0; c < NIXIE_COLON_N; c++) {
    this->_colon[c].setup(NIXIE_COLON_DOT_N, colon[c]);
    for (int d=0; d < NIXIE_COLON_DOT_N; d++) {
      pinMode(colon[c][d], OUTPUT);
      digitalWrite(colon[c][d], LOW);
    }
  } // for(c)
}

NixieTube *NixieArray::get_num() {
  return _num;
}

NixieTube *NixieArray::get_num(uint8_t num_i) {
  return &(_num[num_i]);
}

NixieTube *NixieArray::get_colon() {
  return _colon;
}

// 数字: 個別
void NixieArray::set_num_blightness(uint8_t num_i, uint8_t element_i,
                                    uint8_t blightness) {
  /*
  String str = "NixieArray::set_num_blightness(";
  str += String(num_i) + ",";
  str += String(element_i) + ",";
  str += String(blightness) + ")";
  Serial.println(str);
  */

  this->_num[num_i].set_blightness(element_i, blightness);
}
void NixieArray::set_num_blightness_zero(uint8_t num_i, uint8_t element_i) {
  this->_num[num_i].set_blightness_zero(element_i);
}
void NixieArray::set_num_blightness_max(uint8_t num_i, uint8_t element_i) {
  this->_num[num_i].set_blightness_max(element_i);
}
uint8_t NixieArray::get_num_blightness(uint8_t num_i, uint8_t element_i) {
  return this->_num[num_i].get_blightness(element_i);
}

// 数字: [num_i] 一括
void NixieArray::set_num_blightness(uint8_t num_i,
                                    uint8_t blightness[NIXIE_NUM_DIGIT_N]) {
  this->_num[num_i].set_blightness(blightness);
}
void NixieArray::set_num_blightness_zero(uint8_t num_i) {
  this->_num[num_i].set_blightness_zero();
}
void NixieArray::set_num_blightness_max(uint8_t num_i) {
  this->_num[num_i].set_blightness_max();
}
void NixieArray::get_num_blightness(uint8_t num_i,
                                    uint8_t blightness[NIXIE_NUM_DIGIT_N]) {
  this->_num[num_i].get_blightness(blightness);
}

// 数字: 全体一括
void NixieArray::set_num_blightness(uint8_t blightness[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N]) {
  for (int n=0; n < NIXIE_NUM_N; n++) {
    this->_num[n].set_blightness(blightness[n]);
  } // for (n)
}
void NixieArray::set_num_blightness_zero() {
  for (int n=0; n < NIXIE_NUM_N; n++) {
    this->_num[n].set_blightness_zero();
  } // for (n)
}
void NixieArray::set_num_blightness_max() {
  for (int n=0; n < NIXIE_NUM_N; n++) {
    this->_num[n].set_blightness_max();
  } // for (n)
}
void NixieArray::get_num_blightness(uint8_t blightness[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N]) {
  for (int n; n < NIXIE_NUM_N; n++) {
    for (int d; d < NIXIE_NUM_DIGIT_N; d++) {
      blightness[n][d] = this->_num[n].get_blightness(d);
    } // for (d)
  } // for (n)
}

// コロン: 個別
void NixieArray::set_colon_blightness(uint8_t colon_i, uint8_t dot_i,
                                      uint8_t blightness) {
  this->_colon[colon_i].set_blightness(dot_i, blightness);
}
void NixieArray::set_colon_blightness_zero(uint8_t colon_i, uint8_t dot_i) {
  this->_colon[colon_i].set_blightness_zero(dot_i);
}
void NixieArray::set_colon_blightness_max(uint8_t colon_i, uint8_t dot_i) {
  this->_colon[colon_i].set_blightness_max(dot_i);
}
uint8_t NixieArray::get_colon_blightness(uint8_t colon_i, uint8_t dot_i) {
  return this->_colon[colon_i].get_blightness(dot_i);
}

// コロン: [colon_i] 一括
void NixieArray::set_colon_blightness(uint8_t colon_i,
                                      uint8_t blightness[NIXIE_COLON_DOT_N]) {
  this->_colon[colon_i].set_blightness(blightness);
}
void NixieArray::set_colon_blightness_zero(uint8_t colon_i) {
  this->_colon[colon_i].set_blightness_zero();
}
void NixieArray::set_colon_blightness_max(uint8_t colon_i) {
  this->_colon[colon_i].set_blightness_max();
}
void NixieArray::get_colon_blightness(uint8_t colon_i,
                                      uint8_t blightness[NIXIE_COLON_DOT_N]) {
  this->_colon[colon_i].get_blightness(blightness);
}

// コロン: 全体
void NixieArray::set_colon_blightness(uint8_t blightness[NIXIE_COLON_N][NIXIE_COLON_DOT_N]) {
  for (int c; c < NIXIE_COLON_N; c++) {
    this->_colon[c].set_blightness(blightness[c]);
  } // for (c)
}
void NixieArray::set_colon_blightness_zero() {
  for (int c; c < NIXIE_COLON_N; c++) {
    this->_colon[c].set_blightness_zero();
  } // for (c)
}
void NixieArray::set_colon_blightness_max() {
  for (int c; c < NIXIE_COLON_N; c++) {
    this->_colon[c].set_blightness_max();
  } // for (c)
}
void NixieArray::get_colon_blightness(uint8_t blightness[NIXIE_COLON_N][NIXIE_COLON_DOT_N]) {
  for (int c; c < NIXIE_COLON_N; c++) {
    for (int d; d < NIXIE_COLON_DOT_N; d++) {
      blightness[c][d] = this->_colon[c].get_blightness(d);
    } // for(d)
  } // for (c)
}

// 数字+コロン: 全体
void NixieArray::set_all_blightness_zero() {
  this->set_num_blightness_zero();
  this->set_colon_blightness_zero();
}
void NixieArray::set_all_blightness_max() {
  this->set_num_blightness_max();
  this->set_colon_blightness_max();
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

  this->onoff(timing);

  //--------------------------------------------------------------------------
  // 数字
  for (int p=0; p < pin_n; p++) {
    val[p] = HIGH;
  }

  for (int t=0; t < NIXIE_NUM_N; t++) {
    for (int e=0; e < NIXIE_NUM_DIGIT_N; e++) {
      NixieElement *ne = this->_num[t].get_element(e);
      uint8_t pin = ne->get_pin();

      if ( ne->is_on() ) {
        val[pin] = LOW;
      }
    } // for(e)
  } // for(t)

  for (int p=(pin_n - 1); p >=0; p--) {
    //Serial.print(String(p) + ":" + String(val[p]) + " ");
    
    digitalWrite(_pin_data, val[p]);
    digitalWrite(_pin_clk, HIGH);
    delayMicroseconds(NixieArray::DISP_DELAY_US);

    digitalWrite(_pin_clk, LOW);
    delayMicroseconds(NixieArray::DISP_DELAY_US);
  }
  //Serial.println();
  
  digitalWrite(_pin_stobe, HIGH);
  delayMicroseconds(NixieArray::DISP_DELAY_US);

  digitalWrite(_pin_stobe, LOW);
  delayMicroseconds(NixieArray::DISP_DELAY_US);

  //--------------------------------------------------------------------------
  // コロン
  for (int c=0; c < NIXIE_COLON_N; c++) {
    for (int d=0; d < NIXIE_COLON_DOT_N; d++) {
      NixieElement *cd = this->_colon[c].get_element(d);
      uint8_t pin = cd->get_pin();

      //Serial.print("pin[" + String(pin) + "]:");
      if ( cd->is_on() ) {
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
