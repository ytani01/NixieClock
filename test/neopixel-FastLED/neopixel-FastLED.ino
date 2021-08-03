#include <FastLED.h>

const uint8_t  PIN        = 16;
const uint16_t NUMPIXELS  = 6;
const uint8_t  BRIGHTNESS = 30;
const int      LOOP_DELAY = 1000;

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

  FastLED.addLeds<WS2812B, PIN, GRB>(pixels, NUMPIXELS);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  for (int i=0; i < NUMPIXELS; i++) {
    Serial.printf("loop> i=%d\n", i);

    //portDISABLE_INTERRUPTS();
    pixels[i] = CRGB(Col[Col_i][0], Col[Col_i][1], Col[Col_i][2]);
    //portENABLE_INTERRUPTS();
    FastLED.show();

    Col_i = (Col_i + 1) % COL_N;
    delay(LOOP_DELAY);
  }
}
