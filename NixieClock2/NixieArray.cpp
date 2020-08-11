/*
 *
 */
#include "NixieArray.h"

//============================================================================
void
Nixie::init(uint8_t pin[DIGIT_N]) {
  for (int i=0; i < DIGIT_N; i++) {
    _pin[i] = pin[i];
  }
}

/*
 * 明るさを返す(数字指定)
 */
uint8_t
Nixie::get_blightness(uint8_t digit) {
  return _blightness[digit];
}

/*
 * 明るさを返す(全て)
 */
uint8_t *
Nixie::get_blightness() {
  return _blightness;
}

void
Nixie::set_blightness(uint8_t digit, uint8_t bl) {
  if ( bl >= 0 && bl <= BLIGHTNESS_MAX ) {
    _blightness[digit] = bl;
  }
}

void
Nixie::set_blightness(uint8_t bl[]) {
  for (int d=0; d < DIGIT_N; d++) {
    set_blightness(d, bl[d]);
  } // for(i)
}

uint8_t
Nixie::get_pin(uint8_t digit) {
  return _pin[digit];
}

//============================================================================
void
NixieArray::init(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
		 uint8_t colon_r, uint8_t colon_l, uint8_t led,
		 uint8_t nxa[NIXIE_N][DIGIT_N]) {
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
  Serial.println("sizeof(nxa)=" + String(sizeof(nxa)));
  Serial.println("sizeof(uint8_t)=" + String(sizeof(uint8_t)));

  for (int i=0; i < NIXIE_N; i++) {
    _nx[i].init(nxa[i]);
  }
}

uint8_t
NixieArray::get_pin(uint8_t nixie, uint8_t digit) {
  if (digit < 0 ) {
    return 0;
  }
  return _nx[nixie].get_pin(digit);
}

void
NixieArray::display(unsigned long cur_ms, nixie_disp_data_t *nd) {
  const int pin_n = NIXIE_N * DIGIT_N;
  uint8_t nxa[pin_n]; // nixie array pin values

  for (int i=0; i < pin_n; i++) {
    nxa[i] = HIGH;
  }

  for (int i=0; i < NIXIE_N; i++) {
    uint8_t *d = nd->nx_blightness[i];

    // T.B.D. 明るさ制御方法を見直す
    /***
    if ( cur_ms % 10 < off_timing ) {
      d = DIGIT_BLANK;
    }
    ***/

    // T.B.D. 明るさ制御が必要
    for (int dg=0; dg < DIGIT_N; dg++) {
      unsigned int timing = cur_ms % BLIGHTNESS_MAX;

      if ( _nx[i].get_blightness(dg) > timing ) {
	uint8_t pin = _nx[i].get_pin(dg);
	nxa[pin] = LOW;
      }
    } // for(dg)

    /***
    if ( d == DIGIT_BLANK ) {
      continue;
    }
    if ( d == DIGIT_FOG ) {
      for (int j=0; j < DIGIT_N; j++) {
	nxa[_nx[i].get_pin(j)] = LOW;
      }
      continue;
    }
    ***/
  } // for(i)
  //--------------------------------------------------------------------------
  for (int i=(pin_n - 1); i >= 0; i--) {
    digitalWrite(_pin_data, nxa[i]);
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
  // コロン
  //
  digitalWrite(_pin_colon_r, nd->colon_r);
  digitalWrite(_pin_colon_l, nd->colon_l);
  //--------------------------------------------------------------------------
  // LED
  //
  digitalWrite(_pin_led, nd->led);
}

void
NixieArray::all_off() {
  const int	pin_n = NIXIE_N * DIGIT_N;
  uint8_t	nxa[pin_n];			// nixie array pin values

  for (int i=0; i < pin_n; i++) {
    nxa[i] = HIGH;
  }  

  for (int i=(pin_n - 1); i >= 0; i--) {
    digitalWrite(_pin_data, nxa[i]);
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
