#include <FastLED.h>

const uint8_t  PIN_PIXEL  = 16;
const uint8_t  PIN_PWRLED = 5;
const uint16_t NUMPIXELS  = 6;
const uint8_t  BLIGHTNESS = 60;
const int      LOOP_DELAY = 300;

CRGB pixels[NUMPIXELS];

int Col[][3] =
{
  {BLIGHTNESS,0,0},
  {0,BLIGHTNESS,0},
  {0,0,BLIGHTNESS},
  {BLIGHTNESS,BLIGHTNESS,0},
  {0,BLIGHTNESS,BLIGHTNESS},
  {BLIGHTNESS,0,BLIGHTNESS},
  {BLIGHTNESS,BLIGHTNESS,BLIGHTNESS}
};

const int COL_N = sizeof(Col) / sizeof(Col[0]);
int Col_i = 0;

void setup() {
  Serial.begin(115200);

  Col_i = 0;

  pinMode(PIN_PWRLED, OUTPUT);
  digitalWrite(PIN_PWRLED, LOW);

  FastLED.addLeds<WS2812B, PIN_PIXEL, GRB>(pixels, NUMPIXELS);
  FastLED.setBrightness(BLIGHTNESS);
}

void loop() {
  for (int i=0; i < NUMPIXELS; i++) {
    int cl = (Col_i + i) % COL_N;
    Serial.printf("loop> cl=%d\n", cl);

    //portDISABLE_INTERRUPTS();
    pixels[i] = CRGB(Col[cl][0], Col[cl][1], Col[cl][2]);
    //portENABLE_INTERRUPTS();

  }
  FastLED.show();

  Col_i = (Col_i + 1) % COL_N;
  delay(LOOP_DELAY);
}
