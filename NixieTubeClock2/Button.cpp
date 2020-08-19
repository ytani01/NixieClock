// Button.cpp
// (c) 2018 FabLab Kannai
//
#include "Button.h"

//static unsigned long	Button::Num = 0;
//static Button		*Button::Obj[Button::NUM_MAX];


// Constractor
Button::Button()
{
}

// public methods
void Button::init(byte pin, String name)
{
  _pin          = pin;
  _name         = name;

  _value        = HIGH;
  _prev_value   = HIGH;
  _press_start  = 0;
  _first_press_start = 0;
  _count        = 0;
  _click_count	= 0;
  _long_pressed = false;
  _repeat       = false;

  _is_enabled   = true;

  pinMode(_pin, INPUT_PULLUP);

  //pciSetup(_pin);
}
/*
 * return:
 *	true	changed
 *	false	to be ignored
 */
boolean Button::get()
{
  unsigned long cur_msec = millis();
  boolean 	ret = false;
  
  if ( ! _is_enabled ) {
    return false;
  }

  // is Enabled
  
  _prev_value = _value;
  _value = digitalRead(_pin);

  _click_count = 0;
  if ( _count > 0 ){
    if ( cur_msec - _first_press_start > CLICK_MSEC ) {
      // click count is detected
      _click_count = _count;
      _count = 0;
      ret = true;
    }
  }

  if ( _value == HIGH ) {
    // Released button then refresh some flags and do nothing any more
    _press_start = 0;
    _long_pressed = false;
    _repeat = false;

    if ( _prev_value == LOW) {
      // Released now !
      return true;
    }
    return ret;
  }

  // LOW
  if ( _prev_value == HIGH ) {
    // Pushed now !
    _press_start = cur_msec;
    _count++;
    if ( _count == 1 ) {
      _first_press_start = cur_msec;
    }
    return true;
  }

  // continueing pressed
  if ( ! _long_pressed ) {
    if ( cur_msec - _press_start > LONG_PRESS_MSEC ) {
      _long_pressed = true;
      _press_start = cur_msec;
      return true;
    } else {
      return ret;
    }
  }

  // long pressed
  if ( cur_msec - _press_start > REPEAT_MSEC ) {
    _repeat = true;
    _press_start = cur_msec;
    return true;
  }

  return ret;
}

void Button::enable()
{
  _is_enabled = true;
}
void Button::disable()
{
  _is_enabled = false;
}

boolean Button::isEnabled()
{
  return _is_enabled;
}

String Button::name()
{
  return _name;
}
boolean Button::value()
{
  return _value;
}
count_t Button::count()
{
  return _count;
}
count_t Button::click_count()
{
  return _click_count;
}
boolean Button::long_pressed()
{
  return _long_pressed;
}
boolean Button::repeat()
{
  return _repeat;
}

void Button::print() {
  print(false);
}
void Button::print(boolean interrupt)
{
  String str = interrupt ? "!" : " ";
  
  str += "Btn(" + _name + ") ";
  str += _value        ? "H "  : "L ";
  str += String(_count) + " ";
  str += String(_click_count) + " ";
  str += _long_pressed ? "Long " : "---- ";
  str += _repeat       ? "Repeat "  : "------ ";

  Serial.println(str);
}

// private methods
/*
void Button::pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));   // enable pin
  PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
  PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
*/
