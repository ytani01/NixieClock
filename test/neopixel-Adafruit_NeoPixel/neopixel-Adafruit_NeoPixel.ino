#include <Adafruit_NeoPixel.h>

const uint8_t PIN_ONBOARD_LED = 5;

const uint8_t PIN_PIXEL       = 27;
const uint8_t PIXELS_N        = 6;
const int     BLIGHTNESS      = 255; // 0-255

const int     LOOP_DELAY      = 1000;  // ms

Adafruit_NeoPixel pixels(PIXELS_N, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

const int Col[][3] =
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

  pixels.begin();
  pixels.clear();

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, HIGH);
}

void loop() {
  digitalWrite(PIN_ONBOARD_LED, LOW);
  Serial.printf("loop> cl=[ ");
  for (int i=0; i < PIXELS_N; i++) {
    int cl = (Col_i + i) % COL_N;
    Serial.printf("%d ", cl);
    pixels.setPixelColor(i, pixels.Color(Col[cl][0],
                                         Col[cl][1],
                                         Col[cl][2]));
  }
  pixels.show();
  Serial.printf("]\n");

  Col_i = (Col_i + 1) % COL_N;

  digitalWrite(PIN_ONBOARD_LED, HIGH);
  delay(LOOP_DELAY);
}
