#include "Tubes.h"

#define NUM_DEVICES         3
#define PIN_HV5812_CLK      26
#define PIN_HV5812_STROBE   13
#define PIN_HV5812_DATA     14
#define PIN_HV5812_BLANK    4
#define PIN_COLON_R         16
#define PIN_COLON_L         17
#define PIN_LED             27
#define NUM_DIGITS          6

#define PIN_BUTTON_0 33
#define PIN_BUTTON_1 34
#define PIN_BUTTON_2 35

Tubes tubes;
uint8_t d1[] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1};
uint8_t d2[] = {10, 19, 18, 17, 16, 15, 14, 13, 12, 11};
uint8_t d3[] = {20, 29, 28, 27, 26, 25, 24, 23, 22, 21};
uint8_t d4[] = {30, 39, 38, 37, 36, 35, 34, 33, 32, 31};
uint8_t d5[] = {40, 49, 48, 47, 46, 45, 44, 43, 42, 41};
uint8_t d6[] = {50, 59, 58, 57, 56, 55, 54, 53, 52, 51};
uint8_t *digits[] = {d1, d2, d3, d4, d5, d6};

//unsigned long i = 123456;
unsigned long i = 0;

void setup(){
    tubes.init(NUM_DEVICES, PIN_HV5812_CLK, PIN_HV5812_DATA, PIN_HV5812_STROBE, PIN_HV5812_BLANK);
    tubes.initColon(PIN_COLON_R, PIN_COLON_L);
    tubes.initDigits(NUM_DIGITS);
    Serial.begin(115200);

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        tubes.addDigit(i, digits[i]);
    }

    tubes.clear();
    tubes.enableAll();
}

void loop(){
    tubes.setValue(i);
    tubes.show();

    i++;
    delay(50);
}

void crossfade(){
    bool flg = true;
    uint8_t step = 100;

    tubes.setValue(123456);
    tubes.show();
    for (int i = 0; i < step; i++)
    {
        tubes.blank();
        delay(i * 20 / step);
        tubes.deassertBlank();
        delay(20 - (i * 20 / step));
    }

    tubes.blank();
    tubes.setValue(543210);
    tubes.show();

    for (int i = 0; i < 100; i++)
    {
        tubes.blank();
        delay(20 - (i * 20 / step));
        tubes.deassertBlank();
        delay(i * 20 / step);
    }
}

void randomN(){
    for (int i = 0; i < 100; i++)
    {
        uint8_t idx = random(0, NUM_DIGITS);
        uint8_t n = random(0, 9);
        tubes.setNum(idx, n);
        tubes.enableDigit(idx);
        tubes.show();
        delay(50);
        tubes.disableDigit(idx);
        tubes.show();
    }
}

void chase(){
    tubes.disableAll();
    tubes.setValue(123456);

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        delay(100);
        tubes.enableDigit(i);
        tubes.show();
    }

    delay(3000);

    for (int i = 0; i < NUM_DIGITS; i++)
    {
        delay(100);
        tubes.disableDigit(i);
        tubes.show();
    }

}
