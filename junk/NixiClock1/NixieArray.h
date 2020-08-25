/*
 *
 */
#ifndef NIXIE_ARRAY_H
#define NIXIE_ARRAY_H
#include <Arduino.h>

#define NIXIE_N		6
#define DIGIT_N		10

#define DIGIT_BLANK	10
#define DIGIT_FOG	100

//============================================================================
class Nixie {
 public:
  Nixie() {};
  void init(uint8_t nixie_pin[DIGIT_N]);
  uint8_t get_nixie_pin(uint8_t digit);
  uint8_t get_off_timing();
  void set_off_timing(uint8_t off_timing);

 private:
  uint8_t _digit;
  uint8_t _on_ms;
  uint8_t _off_ms;
  uint8_t _off_timing = 0;
  uint8_t _nixie_pin[DIGIT_N];
};
  
//============================================================================
struct nixie_disp_data_t {
  uint8_t digit[DIGIT_N];
  uint8_t colon_r, colon_l;
  uint8_t led;
};

//============================================================================
class NixieArray {
 public:
  NixieArray(){};
  
  void init(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
	    uint8_t colon_r, uint8_t colon_l, uint8_t led,
	    uint8_t na[NIXIE_N][DIGIT_N]);

  uint8_t get_nixie_pin(uint8_t nixie, uint8_t digit);
  uint8_t get_off_timing(uint8_t nixie);
  void set_off_timing(uint8_t nixie, uint8_t off_timing);

  void display(unsigned long cur_ms, nixie_disp_data_t *nd);
  void all_off();

private:
  Nixie 	_nx[NIXIE_N];
  uint8_t	_pin_clk, _pin_stobe, _pin_data, _pin_blank;
  uint8_t	_pin_colon_r, _pin_colon_l;
  uint8_t	_pin_led;
};

#endif // NIXIE_ARRAY_H
