/*
 * (c) 2020 Yoichi Tanibayashi
 *
 *----------------------------------------------------------------------------
 * [ Data structure ( not class tree ) ]
 *
 *  NixieArray
 *   |
 *   +- NixieTube num[NIXIE_NUM_N]
 *   |   |
 *   |   +- NixieElement element[NIXIE_NUM_DIGIT_N]
 *   |   |
 *   |   +- Effect
 *   |
 *   +- NixieTube colon[NIXIE_COLON_N]
 *       |
 *       +- NixieElement element[NIXIE_COLON_DOT_N]
 *       |
 *       +- Effect
 *----------------------------------------------------------------------------
 */
#ifndef NIXIE_H
#define NIXIE_H
#include <Arduino.h>
#include <RTClib.h>
#include <WiFi.h>

#define BLIGHTNESS_MAX       8

#define NIXIE_NUM_N          6  // 数字の桁数
#define NIXIE_NUM_DIGIT_N   10  // 一桁の数字のパターン数

#define NIXIE_COLON_N        2
#define NIXIE_COLON_R        0
#define NIXIE_COLON_L        1
#define NIXIE_COLON_DOT_N    1  // 現在は、上下同じピンなので、ドットは一つ
#define NIXIE_COLON_DOT_UP   0  // UPとDOWNは、同じ
#define NIXIE_COLON_DOT_DOWN 0  // UPとDOWNは、同じ

#define NIXIE_ELEMENT_N_MAX  (NIXIE_NUM_DIGIT_N > NIXIE_COLON_DOT_N ? NIXIE_NUM_DIGIT_N : NIXIE_COLON_DOT_N)

typedef unsigned long effect_id_t;
#define EFFECT_NONE          0
#define EFFECT_ONLY          1
#define EFFECT_FADEIN        2
#define EFFECT_FADEOUT       3
#define EFFECT_XFADE         4
#define EFFECT_FOGIN         5
#define EFFECT_FOGOUT        6
#define EFFECT_SHUFFLE       7
#define EFFECT_BLINK         8
#define EFFECT_RANDOM_ONOFF  9

//============================================================================
class NixieElement {
 public:
  NixieElement() {};
  
  void setup(uint8_t pin);
  
  void set_blightness(uint8_t blightness);
  void inc_blightness();
  void dec_blightness();
  uint8_t get_blightness();
  
  void set_pin(uint8_t pin);
  uint8_t get_pin();
  
  void on();
  void off();
  boolean is_on();
  
 protected:
  uint8_t _pin = 0;
  uint8_t _blightness = BLIGHTNESS_MAX;
  boolean _on = false;

}; // class NixieElement
//============================================================================
class Effect {
 public:
  Effect(effect_id_t eid, NixieElement *element, unsigned long el_n);
  virtual ~Effect();

  virtual void  start(unsigned long start_ms, unsigned long tick_ms);
  virtual void  start(unsigned long start_ms, unsigned long tick_ms,
                      int element);
  virtual void  start(unsigned long start_ms, unsigned long tick_ms,
                      int el1, int el2);
  virtual void  loop(unsigned long cur_ms);
  virtual void  end();

  effect_id_t get_id();
  boolean     is_active();
  boolean     tick(unsigned long cur_ms);
    
 protected:
  NixieElement *_el;   // array
  unsigned long _el_n      = 0;           // number of elements (10 or 1)
  boolean       _active    = false;
  effect_id_t   _id        = EFFECT_NONE;
  unsigned long _start_ms  = 0;
  unsigned long _tick_ms   = 1000;
  unsigned long _tick      = 0;
  unsigned long _prev_tick = 0;

}; // class Effect
//============================================================================
class EffectOnly : public Effect {
 public:
  EffectOnly(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms, int el_i, int bl);
  void loop(unsigned long cur_ms);

 private:
  int _el_i;
  int _bl;
}; // class EffectOnly
//============================================================================
class EffectFadeIn : public Effect {
 public:
  EffectFadeIn(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms, int element);
  void loop(unsigned long cur_ms);

 private:
  int _el_i;

}; // class EffectFadeIn
//============================================================================
class EffectFadeOut : public Effect {
 public:
  EffectFadeOut(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms, int element);
  void loop(unsigned long cur_ms);

 private:
  int _el_i;

}; // class EffectFadeOut
//============================================================================
class EffectXFade : public Effect {
 public:
  EffectXFade(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms,
             int el_in, int el_out);
  void loop(unsigned long cur_ms);

 private:
  int _el_i_in;
  int _el_i_out;

}; // class EffectXFade

//============================================================================
class EffectShuffle : public Effect {
 public:
  EffectShuffle(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms,
             int n, int el_i);
  void loop(unsigned long cur_ms);
  void end();

 private:
  int   _el_i;
  int   _n;
}; // class EffectShuffle

//============================================================================
class EffectBlink : public Effect {
 public:
  EffectBlink(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms, int el_i);
  void loop(unsigned long cur_ms);
  void end();

 private:
  int      _el_i;
  uint8_t  _blightness[NIXIE_ELEMENT_N_MAX];
  boolean  _onoff;
}; // class EffectBlink

//============================================================================
class EffectRandomOnOff : public Effect {
 public:
  EffectRandomOnOff(NixieElement *el, unsigned long el_n);
  void start(unsigned long start_ms, unsigned long tick_ms, int el_i);
  void loop(unsigned long cur_ms);
  void end();

 private:
  int _el_i;
}; // class EffectRandomOnOff

//============================================================================
class NixieTube {
 public:
  int           element_n;
  NixieElement *element;

  NixieTube() {};

  void setup(int element_n, uint8_t *pin);
  void loop(unsigned long cur_msec);

  Effect *init_effect(effect_id_t eid);
  void end_effect();

  boolean effect_is_active();

  void fadein_start(unsigned long start_ms, unsigned long ms, int el_i);
  void fadeout_start(unsigned long start_ms, unsigned long ms, int el_i);
  void xfade_start(unsigned long start_ms, unsigned long ms,
                   int el_in, int el_out);
  void shuffle_start(unsigned long start_ms, unsigned long tick, int n,
                     int el_i);
  void blink_start(unsigned long start_ms, unsigned long ms, int el_i);
  void randomOnOff_start(unsigned long start_ms, unsigned long ms, int el_i);

 private:
  Effect *_ef;
}; // class NixieTube
//============================================================================
class NixieArray {
 public:
  static const unsigned int DISP_DELAY_US = 1; // microsec

  NixieTube  num[NIXIE_NUM_N];
  NixieTube  colon[NIXIE_COLON_N];

  NixieArray(uint8_t clk, uint8_t stobe, uint8_t data, uint8_t blank,
             uint8_t num[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N],
             uint8_t colon[NIXIE_COLON_N][NIXIE_COLON_DOT_N]);
  void loop(unsigned long cur_ms);

  void end_all_effect();
  
  void set_onoff(unsigned long cur_ms);
  void display(unsigned long cur_ms);

 private:
  uint8_t    _pin_clk, _pin_stobe, _pin_data, _pin_blank;
};
//============================================================================
#endif // NIXIE_H

// for emacs ..
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End:
