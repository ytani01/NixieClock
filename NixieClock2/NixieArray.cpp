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
		 uint8_t nxa[NIXIE_N][DIGIT_N],
		 uint8_t colon[COLON_N],
		 uint8_t led) {
  //--------------------------------------------------------------------------
  _pin_clk = clk;
  _pin_stobe = stobe;
  _pin_data = data;
  _pin_blank = blank;

  uint8_t pins_out[] = {_pin_clk, _pin_stobe, _pin_data, _pin_blank};
  for (int p=0; p < sizeof(pins_out) / sizeof(uint8_t); p++) {
    pinMode(pins_out[p], OUTPUT);
    digitalWrite(pins_out[p], LOW);
  } // for(p)

  for (int i=0; i < NIXIE_N; i++) {
    _nx[i].init(nxa[i]);
  }
  //--------------------------------------------------------------------------
  for (int c=0; c < COLON_N; c++) {
    _pin_colon[c] = colon[c];
    digitalWrite(_pin_colon[c], LOW);
  }
  //--------------------------------------------------------------------------
  _pin_led = led;
  digitalWrite(_pin_led, LOW);    
  //--------------------------------------------------------------------------
}

Nixie
NixieArray::get_nixie(uint8_t i) {
  return _nx[i];
}

uint8_t
NixieArray::get_pin(uint8_t nixie, uint8_t digit) {
  if (digit < 0 ) {
    return 0;
  }
  return _nx[nixie].get_pin(digit);
}

void
NixieArray::set_nixie(uint8_t blightness[NIXIE_N][DIGIT_N]) {
  for (int nx=0; nx < NIXIE_N; nx++) {
    _nx[nx].set_blightness(blightness[nx]);
  }
}

void
NixieArray::set_nixie(uint8_t nixie, uint8_t blightness[DIGIT_N]) {
  _nx[nixie].set_blightness(blightness);
}

void
NixieArray::set_nixie(uint8_t nixie, uint8_t digit, uint8_t blightness) {
  _nx[nixie].set_blightness(digit, blightness);
}

void
NixieArray::set_colon(uint8_t blightness[COLON_N]) {
}

void
NixieArray::set_led(uint8_t blightness) {
} 

void
NixieArray::display(unsigned long cur_ms, nixie_disp_data_t *nd) {
  const int pin_n = NIXIE_N * DIGIT_N;
  uint8_t nxa[pin_n]; // nixie array pin values

  //--------------------------------------------------------------------------
  /*
   * ニキシー管の点灯
   */
  for (int i=0; i < pin_n; i++) {
    nxa[i] = HIGH;
  }
  
  set_nixie(nd->nx_blightness);
  
  for (int i=0; i < NIXIE_N; i++) {
    uint8_t *d = nd->nx_blightness[i];
    
    for (int dg=0; dg < DIGIT_N; dg++) {
      unsigned int timing = cur_ms % BLIGHTNESS_MAX;

      if ( _nx[i].get_blightness(dg) > timing ) {
	uint8_t pin = _nx[i].get_pin(dg);
	nxa[pin] = LOW;
      }
    } // for(dg)
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
  /*
   * コロン
   */
  for (int c=0; c < COLON_N; c++) {
    digitalWrite(_pin_colon[c], nd->colon[c]);
  }
  //--------------------------------------------------------------------------
  /*
   * LED
   */
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
    // delay(1);
    digitalWrite(_pin_clk, LOW);
    // delay(1);
  }

  digitalWrite(_pin_stobe, HIGH);
  // delay(1);
  digitalWrite(_pin_stobe, LOW);
  // delay(1);
}
