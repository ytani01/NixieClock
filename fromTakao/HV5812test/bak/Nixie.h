/*
 *
 */
#ifndef NIXIE_H
#define NIXIE_H
#include <Arduino.h>

class Nixie {
 public:
  static const unsigned long DIGIT_N = 10;
  
  Nixie(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4,
	uint8_t d5, uint8_t d6, uint8_t d7, uint8_t d8, uint8_t d9);

  uint8_t digit(uint8_t n);
  
 private:
  uint8_t	_digit[DIGIT_N];
  
};

#endif // NIXIE_H
