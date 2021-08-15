#include <Adafruit_NeoPixel.h>

const uint8_t PIN_PIXEL  = 16;
const uint8_t PIXELS_N   = 6;
const int     LOOP_DELAY = 500;  // ms

Adafruit_NeoPixel pixels(PIXELS_N, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

const int BLIGHTNESS = 30;
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

  pinMode(5, OUTPUT);

  pixels.begin();
  digitalWrite(5, HIGH);

  pixels.clear();
}

void loop() {
  for (int i=0; i < PIXELS_N; i++) {
    int cl = (Col_i + i) % COL_N;
    Serial.printf("loop> cl=%d\n", cl);
    pixels.setPixelColor(i, pixels.Color(Col[cl][0],
                                         Col[cl][1],
                                         Col[cl][2]));
  }
  pixels.show();

  Col_i++;
  delay(LOOP_DELAY);
}
