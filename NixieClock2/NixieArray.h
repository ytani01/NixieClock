/*
 *
 */
#ifndef NIXIE_ARRAY_H
#define NIXIE_ARRAY_H
#include <Arduino.h>

#define NIXIE_N		6
#define DIGIT_N		10

#define BLIGHTNESS_MAX  10

#define DIGIT_BLANK	10
#define DIGIT_FOG	100
//============================================================================
struct nixie_disp_data_t {
  uint8_t nx_blightness[NIXIE_N][DIGIT_N];
  uint8_t colon_r, colon_l;
  uint8_t led;
};
//============================================================================
/**
 * ニキシー管1本
 */
class Nixie {
 public:
  Nixie() {};
  void init(uint8_t pin[DIGIT_N]);

  uint8_t get_pin(uint8_t digit);

  uint8_t get_blightness(uint8_t digit);
  uint8_t *get_blightness();
  void set_blightness(uint8_t digit, uint8_t bl);
  void set_blightness(uint8_t bl[]);
  
 private:
  uint8_t _pin[DIGIT_N];	// 数字に対応するピン番号
  uint8_t _blightness[DIGIT_N];	// 数字毎の明るさ(0-5)
};
//============================================================================
class NixieArray {
 public:
  NixieArray(){};

  void init(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
	    uint8_t colon_r, uint8_t colon_l, uint8_t led,
	    uint8_t nxa[NIXIE_N][DIGIT_N]);

  uint8_t get_pin(uint8_t nixie, uint8_t digit);

  void display(unsigned long cur_ms, nixie_disp_data_t *nd);
  void all_off();

private:
  Nixie 	_nx[NIXIE_N];
  uint8_t	_pin_clk, _pin_stobe, _pin_data, _pin_blank;
  uint8_t	_pin_colon_r, _pin_colon_l;
  uint8_t	_pin_led;
};
//============================================================================
#endif // NIXIE_ARRAY_H
