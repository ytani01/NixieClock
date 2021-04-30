/*
 * (c) 2021 Yoichi Tanibayashi
 */
#include "NixieElement.h"

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
void NixieElement::inc_blightness() {
  if (this->_blightness < BLIGHTNESS_RESOLUTION) {
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

// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
