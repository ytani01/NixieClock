/*
 *
 */
#include "Nixie.h"

Nixie::Nixie(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4,
	     uint8_t d5, uint8_t d6, uint8_t d7, uint8_t d8, uint8_t d9) {
  _digit[0] = d0;
  _digit[1] = d1;
  _digit[2] = d2;
  _digit[3] = d3;
  _digit[4] = d4;
  _digit[5] = d5;
  _digit[6] = d6;
  _digit[7] = d7;
  _digit[8] = d8;
  _digit[9] = d9;
}

uint8_t Nixie::digit(uint8_t n) {
  return _digit[n];
}
