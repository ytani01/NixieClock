/*
 *
 * Data structure ( not class tree )
 *
 *  NixieArray
 *   |
 *   +- NixieNum[NIXIE_NUM_N]
 *   |   |
 *   |   +- NixieNumDigit[NIXIE_NUM_DIGIT_N]
 *   |
 *   +- NixieColon[NIXIE_COLON_N]
 *
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

//============================================================================
class NixieElement {
 public:
  NixieElement() {};
  
  void init(uint8_t pin);
  
  void set_blightness(uint8_t blightness);
  uint8_t get_blightness();
  
  void set_pin(uint8_t pin);
  uint8_t get_pin();
  
  void onoff(uint8_t timing);
  boolean is_on();
  
 protected:
  uint8_t _pin = 0;
  uint8_t _blightness = BLIGHTNESS_MAX;
  boolean _on = true;
};
//============================================================================
class NixieNumDigit : public NixieElement {};
//============================================================================
class NixieColon : public NixieElement {};
//============================================================================
class NixieNum {
 public:
  NixieNum() {};

  NixieNumDigit *get_digit();
  NixieNumDigit *get_digit(uint8_t digit_i);

  uint8_t get_blightness(uint8_t digit_i);
  
  void onoff(uint8_t timing);
  
 private:
  NixieNumDigit _digit[NIXIE_NUM_DIGIT_N];
};
//============================================================================
class NixieArray {
 public:
  NixieArray() {};
  
  void init(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
            uint8_t num[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
            uint8_t colon[NIXIE_COLON_N]);
    
  NixieNum *get_num();
  NixieNum *get_num(uint8_t num_i);
  
  NixieColon *get_colon();

  uint8_t get_num_blightness(uint8_t num_i, uint8_t digit_i);
  void set_num_blightness(uint8_t num_i, uint8_t digit_i, uint8_t blightness);

  void set_colon_blightness(uint8_t colon_i, uint8_t blightness);

  void onoff(uint8_t timing);

  void display(unsigned long cur_msec);

 private:
  uint8_t    _pin_clk, _pin_stobe, _pin_data, _pin_blank;
  NixieNum   _num[NIXIE_NUM_N];
  NixieColon _colon[NIXIE_COLON_N];
};
//============================================================================
#endif // NIXIE_ARRAY_H
