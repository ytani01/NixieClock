/**
 * (c) 2021 Yoichi Tanibayashi
 *
 *----------------------------------------------------------------------------
 * [ Data structure ( not a class tree ) ]
 *
 *  NixieArray
 *   |
 *   +- NixieTube num[NIXIE_NUM_N]
 *   |   |
 *   |   +- NixieElement element[NIXIE_NUM_DIGIT_N]
 *   |   |
 *   |   +- Effect
 *   |
 *   +- NixieTube colon[NIXIE_COLON_N]
 *       |
 *       +- NixieElement element[NIXIE_COLON_DOT_N]
 *       |
 *       +- Effect
 *----------------------------------------------------------------------------
 */
#ifndef NIXIE_ELEMENT_H
#define NIXIE_ELEMENT_H

#include "Arduino.h"
#include "Nixie.h"

class NixieElement {
 public:
  NixieElement() {};
  
  void setup(uint8_t pin);
  
  void set_blightness(uint8_t blightness);
  void inc_blightness();
  void dec_blightness();
  unsigned int get_blightness();

  
  void set_pin(uint8_t pin);
  uint8_t get_pin();
  
  void on();
  void off();
  boolean is_on();
  
 protected:
  uint8_t _pin = 0;
  unsigned int _blightness = 8;
  boolean _on = false;

}; // class NixieElement
#endif // NIXIE_ELEMENT_H
