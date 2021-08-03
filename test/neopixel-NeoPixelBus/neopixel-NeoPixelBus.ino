#include <NeoPixelBus.h>

const uint16_t PIXEL_N    = 6;
const uint8_t  PIN        = 16;
const uint8_t  BLIGHTNESS = 30;
const int      LOOP_DELAY = 1000;

//NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels(PIXEL_N, PIN);
NeoPixelBus<NeoGrbFeature, NeoWs2812xMethod > pixels(PIXEL_N, PIN);

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
  while(!Serial) {
  }
  Serial.println("Serial");
  
  Col_i = 0;

  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);

  pinMode(PIN, OUTPUT);
  pixels.Begin();
  while (! pixels.CanShow()) {
    Serial.print(".");
    delay(1);
  }
  pixels.Show();
  Serial.println("Show0");

  Serial.println("AAA");

  pixels.SetPixelColor(0, RgbColor(20));
  while (! pixels.CanShow()) {
    Serial.print(".");
    delay(1);
  }
  Serial.println("Show");
  pixels.Show();

  Serial.println("BBB");
  delay(10000);

  Serial.println("AAA");

  pixels.SetPixelColor(1, RgbColor(20));
  while (! pixels.CanShow()) {
    Serial.print(".");
    delay(1);
  }
  Serial.println("Show");
  pixels.Show();

  Serial.println("BBB");
  delay(10000);

  for (int i=0; i < PIXEL_N; i++) {
    pixels.SetPixelColor(i, RgbColor(20));
    while (! pixels.CanShow()) {
      Serial.print(".");
      delay(1);
    }
    Serial.println("Show");
    pixels.Show();
    delay(LOOP_DELAY);
  }
}

void loop() {
  /*
  for (int i=0; i < PIXEL_N; i++) {
    Serial.printf("loop> i=%d\n", i);
    pixels.SetPixelColor(i, RgbColor(Col[Col_i][0],
                                     Col[Col_i][1],
                                     Col[Col_i][2]));
    pixels.Show();

    Col_i = (Col_i + 1) % COL_N;
    delay(LOOP_DELAY);
  }
  */
  while (! pixels.CanShow()) {
    Serial.print('.');
    delay(1);
  }
  RgbColor c = pixels.GetPixelColor(0);
  uint8_t b = c.CalculateBrightness();

  Serial.printf("b=%d\n", b);

  pixels.Show();
  delay(LOOP_DELAY);
}
