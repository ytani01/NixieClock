/*
 * (c) 2020 Yoichi Tanibayashi
 *
 * [ Data structure ( not class tree ) ]
 *
 *  NixieArray
 *   |
 *   +- NixieTube[NIXIE_NUM_N]
 *   |   |
 *   |   +- NixieElement _digit[NIXIE_NUM_DIGIT_N]
 *   |
 *   +- NixieTube[NIXIE_COLON_N]
 *       |
 *       +- NixieElement _dot[NIXIE_COLON_DOT_N]
 */
#ifndef NIXIE_ARRAY_H
#define NIXIE_ARRAY_H
#include <Arduino.h>

typedef uint8_t effect_t;

#define BLIGHTNESS_MAX       8

#define NIXIE_NUM_N          6
#define NIXIE_NUM_DIGIT_N   10

#define NIXIE_COLON_N        2
#define NIXIE_COLON_DOT_N    2
#define COLON_R              0
#define COLON_L              1

#define NIXIE_ELEMENT_N_MAX  (NIXIE_NUM_DIGIT_N > NIXIE_COLON_DOT_N ? NIXIE_NUM_DIGIT_N : NIXIE_COLON_DOT_N)

#define EFFECT_NONE          0
#define EFFECT_FADE_IN       1
#define EFFECT_FADE_OUT      2
#define EFFECT_X_FADE        3
#define EFFECT_FOG_IN        4
#define EFFECT_FOG_OUT       5
#define EFFECT_SHUFFLE       6
#define EFFECT_BLINK         7

//============================================================================
class NixieElement {
 public:
  NixieElement() {};
  
  void setup(uint8_t pin);
  
  void set_blightness(uint8_t blightness);
  void set_blightness_zero();
  void set_blightness_max();
  void inc_blightness();
  void dec_blightness();
  uint8_t get_blightness();
  
  void set_pin(uint8_t pin);
  uint8_t get_pin();
  
  void on();
  void off();
  boolean is_on();
  
 protected:
  uint8_t _pin = 0;
  uint8_t _blightness = BLIGHTNESS_MAX;
  boolean _on = false;
}; // class NixieElement
//============================================================================
class NixieTube {
 public:
  int           element_n;
  NixieElement  *element;

  NixieTube() {};

  void setup(int element_n, uint8_t *pin);
  
  void loop(unsigned long cur_msec);

 private:
  effect_t      _effect = EFFECT_NONE;
  uint8_t       _effect_param_element1, _effect_param_element2;
  unsigned long _effect_param_msec1;
}; // class NixieTube
//============================================================================
class NixieArray {
 public:
  static const unsigned int DISP_DELAY_US = 1; // microsec

  NixieTube  num[NIXIE_NUM_N];
  NixieTube  colon[NIXIE_COLON_N];

  NixieArray() {};
  
  void setup(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
             uint8_t num[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
             uint8_t colon[NIXIE_COLON_N][NIXIE_COLON_DOT_N]);
    
  void loop(unsigned long cur_ms);

  void set_onoff(unsigned long cur_ms);

  void display(unsigned long cur_ms);

 private:
  uint8_t    _pin_clk, _pin_stobe, _pin_data, _pin_blank;
};
//============================================================================
#endif // NIXIE_ARRAY_H

// for emacs ..
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
