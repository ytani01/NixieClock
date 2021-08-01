#include <FastLED.h>

#define PIN   15
#define NUMPIXELS 6
#define BRIGHTNESS 64

CRGB pixels[NUMPIXELS];

int Col[][3] =
{
  {50,0,0},
  {0,50,0},
  {0,0,50},
  {50,50,0},
  {0,50,50},
  {50,0,50},
  {50,50,50}
};

const int COL_N = sizeof(Col) / sizeof(Col[0]);
int Col_i = 0;

void setup() {
  Serial.begin(115200);

  Col_i = 0;

  pinMode(5, OUTPUT);

  digitalWrite(5, HIGH);

  FastLED.addLeds<WS2812B, PIN, RGB>(pixels, NUMPIXELS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for (int i=0; i < NUMPIXELS; i++) {
    Serial.printf("loop> i=%d\n", i);

    CRGB(Col[Col_i][0], Col[Col_i][1], Col[Col_i][2]);
    FastLED.show();

    Col_i = (Col_i + 1) % COL_N;
    delay(500);
  }
}
