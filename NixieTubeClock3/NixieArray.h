/*
 * (c) 2020 Yoichi Tanibayashi
 *
 * [ Data structure ( not class tree ) ]
 *
 *  NixieArray
 *   |
 *   +- NixieNum[NIXIE_NUM_N]
 *   |   |
 *   |   +- NixieElement _digit[NIXIE_NUM_DIGIT_N]
 *   |
 *   +- NixieColon[NIXIE_COLON_N]
 *       |
 *       +- NixieElement _dot[NIXIE_COLON_DOT_N]
 */
#ifndef NIXIE_ARRAY_H
#define NIXIE_ARRAY_H
#include <Arduino.h>

#define BLIGHTNESS_MAX       8

#define NIXIE_NUM_N          6
#define NIXIE_NUM_DIGIT_N   10

#define NIXIE_COLON_N        2
#define NIXIE_COLON_DOT_N    2
#define COLON_R              0
#define COLON_L              1

#define NIXIE_ELEMENT_N_MAX  (NIXIE_NUM_DIGIT_N > NIXIE_COLON_DOT_N ? NIXIE_NUM_DIGIT_N : NIXIE_COLON_DOT_N)

//============================================================================
class NixieElement {
 public:
  NixieElement() {};
  
  void setup(uint8_t pin);
  
  void set_blightness(uint8_t blightness);
  void set_blightness_zero();
  void set_blightness_max();
  uint8_t get_blightness();
  
  void set_pin(uint8_t pin);
  uint8_t get_pin();
  
  void onoff(uint8_t timing);
  boolean is_on();
  
 protected:
  uint8_t _pin = 0;
  uint8_t _blightness = BLIGHTNESS_MAX;
  boolean _on = false;
}; // class NixieElement
//============================================================================
class NixieTube {
 public:
  NixieTube() {};

  void setup(int element_n, uint8_t *pin);
  
  NixieElement *get_element();
  NixieElement *get_element(uint8_t element_i);

  void set_blightness(uint8_t element_i, uint8_t blightness);
  void set_blightness_zero(uint8_t element_i);
  void set_blightness_max(uint8_t element_i);
  uint8_t get_blightness(uint8_t element_i);

  void set_blightness(uint8_t *blightness);
  void set_blightness_zero();
  void set_blightness_max();
  void get_blightness(uint8_t *blightness);
  
  void onoff(uint8_t timing);
  
 private:
  NixieElement *_element;
}; // class NixieTube
//============================================================================
class NixieArray {
 public:
  static const unsigned int DISP_DELAY_US = 1; // microsec

  NixieArray() {};
  
  void setup(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
             uint8_t num[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
             uint8_t colon[NIXIE_COLON_N][NIXIE_COLON_DOT_N]);
    
  NixieTube *get_num();
  NixieTube *get_num(uint8_t num_i);
  
  NixieTube *get_colon();

  void set_num_blightness(uint8_t num_i, uint8_t element_i,
                          uint8_t blightness);
  void set_num_blightness_zero(uint8_t num_i, uint8_t element_i);
  void set_num_blightness_max(uint8_t num_i, uint8_t element_i);
  uint8_t get_num_blightness(uint8_t num_i, uint8_t element_i);

  void set_num_blightness(uint8_t num_i,
                          uint8_t blightness[NIXIE_NUM_DIGIT_N]);
  void set_num_blightness_zero(uint8_t num_i);
  void set_num_blightness_max(uint8_t num_i);
  void get_num_blightness(uint8_t num_i, uint8_t blightness[NIXIE_NUM_DIGIT_N]);

  void set_num_blightness(uint8_t blightness[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N]);
  void set_num_blightness_zero();
  void set_num_blightness_max();
  void get_num_blightness(uint8_t blightness[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N]);

  void set_colon_blightness(uint8_t colon_i, uint8_t dot_i, uint8_t blightness);
  void set_colon_blightness_zero(uint8_t colon_i, uint8_t dot_i);
  void set_colon_blightness_max(uint8_t colon_i, uint8_t dot_i);
  uint8_t get_colon_blightness(uint8_t colon_i, uint8_t dot_i);

  void set_colon_blightness(uint8_t colon_i,
                            uint8_t blightness[NIXIE_COLON_DOT_N]);
  void set_colon_blightness_zero(uint8_t colon_i);
  void set_colon_blightness_max(uint8_t colon_i);
  void get_colon_blightness(uint8_t colon_i,
                            uint8_t blightness[NIXIE_COLON_DOT_N]);

  void set_colon_blightness(uint8_t blightness[NIXIE_COLON_N][NIXIE_COLON_DOT_N]);
  void set_colon_blightness_zero();
  void set_colon_blightness_max();
  void get_colon_blightness(uint8_t blightness[NIXIE_COLON_N][NIXIE_COLON_DOT_N]);

  void set_all_blightness_zero();
  void set_all_blightness_max();
                            
  void onoff(uint8_t timing);

  void display(unsigned long cur_msec);

 private:
  uint8_t    _pin_clk, _pin_stobe, _pin_data, _pin_blank;
  NixieTube  _num[NIXIE_NUM_N];
  NixieTube  _colon[NIXIE_COLON_N];
};
//============================================================================
#endif // NIXIE_ARRAY_H

// for emacs ..
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
