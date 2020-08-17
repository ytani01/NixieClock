/*
 *
 * Data structure ( not class tree )
 *
 *  NixieTubeArray
 *   |
 *   +- NixieTube[NIXIE_TUBE_N]
 *   |   |
 *   |   +- NixieTubeDigit[NIXIE_TUBE_DIGIT_N]
 *   |
 *   +- NixieColon[NIXIE_COLON_N]
 *
 */
#ifndef NIXIE_TUBE_ARRAY_H
#define NIXIE_TUBE_ARRAY_H
#include <Arduino.h>

#define BLIGHTNESS_MAX        8

#define NIXIE_TUBE_N          6
#define NIXIE_TUBE_DIGIT_N   10

#define NIXIE_COLON_N         2
#define COLON_R               0
#define COLON_L               1

//============================================================================
class NixieElement {
 public:
  NixieElement() {};
  
  void init(uint8_t pin);
  
  void set_blightness(uint8_t blightness);
  uint8_t get_blightness();
  
  void set_pin(uint8_t pin);
  uint8_t get_pin();
  
  void set_onoff(uint8_t timing);
  boolean get_cur_on();
  
 protected:
  uint8_t _pin = 0;
  uint8_t _blightness = BLIGHTNESS_MAX;
  boolean _cur_on = true;
};
//============================================================================
class NixieTubeDigit : public NixieElement {
};
//============================================================================
class NixieColon : public NixieElement {
};
//============================================================================
class NixieTube {
 public:
  NixieTube() {};

  NixieTubeDigit *get_nx_digit();
  NixieTubeDigit *get_nx_digit(uint8_t digit_i);

  uint8_t get_blightness(uint8_t digit_i);
  
  void set_onoff(uint8_t timing);
  
 private:
  NixieTubeDigit _nx_digit[NIXIE_TUBE_DIGIT_N];
};
//============================================================================
class NixieTubeArray {
 public:
  NixieTubeArray() {};
  
  void init(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
            uint8_t nixie[NIXIE_TUBE_N][NIXIE_TUBE_DIGIT_N],
            uint8_t colon[NIXIE_COLON_N]);
    
  NixieTube *get_nx_tube();
  NixieTube *get_nx_tube(uint8_t tube_i);
  
  NixieColon *get_nx_colon();

  uint8_t get_tube_blightness(uint8_t tube_i, uint8_t digit_i);

  void set_tube_blightness(uint8_t tube_i, uint8_t digit_i,
                           uint8_t blightness);
  void set_colon_blightness(uint8_t colon_i, uint8_t blightness);

  void set_onoff(uint8_t timing);

  void display(unsigned long cur_msec);

 private:
  uint8_t     _pin_clk, _pin_stobe, _pin_data, _pin_blank;
  NixieTube   _nx_tube[NIXIE_TUBE_N];
  NixieColon  _nx_colon[NIXIE_COLON_N];
};
//============================================================================
#endif // NIXIE_TUBE_ARRAY_H
