#include <NeoPixelBus.h>

const uint8_t  PIN_ONBOARD_LED = 5;

const uint8_t  PIN_PIXEL       = 16;
const uint16_t PIXEL_N         = 6 + 7;
const int      LOOP_DELAY      = 1000; // ms

const int      BLIGHTNESS      = 10; // 0-255

NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod > pixels(PIXEL_N, PIN_PIXEL);

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

  pinMode(PIN_PIXEL, OUTPUT);
  pixels.Begin();

  pinMode(PIN_ONBOARD_LED, OUTPUT);
  digitalWrite(PIN_ONBOARD_LED, HIGH);
}

void loop() {
  digitalWrite(PIN_ONBOARD_LED, LOW);
  Serial.printf("loop> cl=[ ");
  for (int i=0; i < PIXEL_N; i++) {
    int cl = (Col_i + i) % COL_N;
    Serial.printf("%d ", cl);
    pixels.SetPixelColor(i, RgbColor(Col[cl][0],
                                     Col[cl][1],
                                     Col[cl][2]));
  }
  pixels.Show();
  Serial.printf("]\n");

  Col_i = (Col_i + 1) % COL_N;

  digitalWrite(PIN_ONBOARD_LED, HIGH);
  delay(LOOP_DELAY);
}
