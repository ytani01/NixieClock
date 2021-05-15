/*
 * (c) 2020 Yoichi Tanibayashi
 *
 *----------------------------------------------------------------------------
 * [ Data structure ( not a class tree ) ]
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

#define BLIGHTNESS_RESOLUTION      16  // 明るさの段階数 2^n

#define NIXIE_NUM_N          6  // 数字の桁数
#define NIXIE_NUM_DIGIT_N   10  // 一桁の数字のパターン数

#define NIXIE_COLON_N        2
#define NIXIE_COLON_R        0
#define NIXIE_COLON_L        1
#define NIXIE_COLON_DOT_N    1  // 現在は、上下同じピンなので、ドットは一つ
#define NIXIE_COLON_DOT_UP   0  // UPとDOWNは、同じ
#define NIXIE_COLON_DOT_DOWN 0  // UPとDOWNは、同じ

#define NIXIE_ELEMENT_N_MAX  (NIXIE_NUM_DIGIT_N > NIXIE_COLON_DOT_N ? NIXIE_NUM_DIGIT_N : NIXIE_COLON_DOT_N)

#endif // NIXIE_H

// for emacs ..
// Local Variables:
// Mode: arduino
// Coding: utf-8-unix
// End: