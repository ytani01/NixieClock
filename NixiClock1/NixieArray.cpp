/*
 *
 */
#include "NixieArray.h"

//============================================================================
void
Nixie::init(uint8_t nixie_pin[DIGIT_N]) {
  for (int i=0; i < DIGIT_N; i++) {
    _nixie_pin[i] = nixie_pin[i];
  }
  _off_timing = 0;
}

uint8_t
Nixie::get_nixie_pin(uint8_t digit) {
  return _nixie_pin[digit];
}

uint8_t
Nixie::get_off_timing() {
  return _off_timing;
}

void
Nixie::set_off_timing(uint8_t off_timing) {
  _off_timing = off_timing;
}
//============================================================================
void
NixieArray::init(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
		 uint8_t colon_r, uint8_t colon_l, uint8_t led,
		 uint8_t na[NIXIE_N][DIGIT_N]) {
  _pin_clk = clk;
  _pin_stobe = stobe;
  _pin_data = data;
  _pin_blank = blank;
  _pin_colon_r = colon_r;
  _pin_colon_l = colon_l;
  _pin_led = led;
  
  uint8_t pins_out[] = {_pin_clk, _pin_stobe, _pin_data, _pin_blank,
			_pin_colon_r, _pin_colon_l, _pin_led};

  for (int p=0; p < sizeof(pins_out)/sizeof(uint8_t); p++) {
    pinMode(pins_out[p], OUTPUT);
    digitalWrite(pins_out[p], LOW);
  }
  Serial.println("sizeof(na)=" + String(sizeof(na)));
  Serial.println("sizeof(uint8_t)=" + String(sizeof(uint8_t)));

  for (int i=0; i < NIXIE_N; i++) {
    _nx[i].init(na[i]);
  }
}

uint8_t
NixieArray::get_nixie_pin(uint8_t nixie, uint8_t digit) {
  if (digit < 0 ) {
    return 0;
  }
  return _nx[nixie].get_nixie_pin(digit);
}

uint8_t
NixieArray::get_off_timing(uint8_t nixie) {
  return _nx[nixie].get_off_timing();
}

void
NixieArray::set_off_timing(uint8_t nixie, uint8_t off_timing) {
  _nx[nixie].set_off_timing(off_timing);
}

void
NixieArray::display(unsigned long cur_ms, nixie_disp_data_t *nd) {
  const int nixie_pin_n = NIXIE_N * DIGIT_N;
  uint8_t na[nixie_pin_n]; // nixie array pin values

  for (int i=0; i < nixie_pin_n; i++) {
    na[i] = HIGH;
  }

  for (int i=0; i < NIXIE_N; i++) {
    uint8_t d = nd->digit[i];
    uint8_t off_timing = _nx[i].get_off_timing();
    uint8_t nixie_pin = _nx[i].get_nixie_pin(d);

    if ( cur_ms % 10 < off_timing ) {
      d = DIGIT_BLANK;
    }

    if ( d >= 0 && d <= 9 ) {
      na[nixie_pin] = LOW;
      continue;
    }
    if ( d == DIGIT_BLANK ) {
      continue;
    }
    if ( d == DIGIT_FOG ) {
      for (int j=0; j < DIGIT_N; j++) {
	na[_nx[i].get_nixie_pin(j)] = LOW;
      }
      continue;
    }
  } // for(i)
  //--------------------------------------------------------------------------
  for (int i=(nixie_pin_n - 1); i >= 0; i--) {
    digitalWrite(_pin_data, na[i]);
    digitalWrite(_pin_clk, HIGH);
    //delay(1);
    digitalWrite(_pin_clk, LOW);
    //delay(1);
  }

  digitalWrite(_pin_stobe, HIGH);
  //delay(1);
  digitalWrite(_pin_stobe, LOW);
  //delay(1);
  //--------------------------------------------------------------------------
  digitalWrite(_pin_colon_r, nd->colon_r);
  digitalWrite(_pin_colon_l, nd->colon_l);
  //--------------------------------------------------------------------------
  digitalWrite(_pin_led, nd->led);
}

void
NixieArray::all_off() {
  const int nixie_pin_n = NIXIE_N * DIGIT_N;
  uint8_t na[nixie_pin_n]; // nixie array pin values

  for (int i=0; i < nixie_pin_n; i++) {
    na[i] = HIGH;
  }  

  for (int i=(nixie_pin_n - 1); i >= 0; i--) {
    digitalWrite(_pin_data, na[i]);
    digitalWrite(_pin_clk, HIGH);
    //delay(1);
    digitalWrite(_pin_clk, LOW);
    //delay(1);
  }

  digitalWrite(_pin_stobe, HIGH);
  //delay(1);
  digitalWrite(_pin_stobe, LOW);
  //delay(1);
}
