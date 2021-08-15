#include <NeoPixelBus.h>

const uint8_t  PIN_PIXEL  = 16;
const uint8_t  PIN_PWRLED = 5;
const uint16_t PIXEL_N    = 6;
const int      LOOP_DELAY = 300;

//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels(PIXEL_N, PIN_PIXEL);
NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod > pixels(PIXEL_N, PIN_PIXEL);

const int BLIGHTNESS = 15;
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

  pinMode(PIN_PWRLED, OUTPUT);
  digitalWrite(PIN_PWRLED, LOW);

  pinMode(PIN_PIXEL, OUTPUT);

  pixels.Begin();
}

void loop() {
  digitalWrite(PIN_PWRLED, HIGH);
  for (int i=0; i < PIXEL_N; i++) {
    int cl = (Col_i + i) % COL_N;
    Serial.printf("loop> cl=%d\n", cl);
    pixels.SetPixelColor(i, RgbColor(Col[cl][0],
                                     Col[cl][1],
                                     Col[cl][2]));
  }
  pixels.Show();

  Col_i = (Col_i + 1) % COL_N;

  digitalWrite(PIN_PWRLED, LOW);
  delay(LOOP_DELAY);
}
