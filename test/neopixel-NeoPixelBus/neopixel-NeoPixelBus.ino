#include <NeoPixelBus.h>

const uint16_t PIXEL_N = 150;
const uint8_t  PIN = 14;


NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> pixels(PIXEL_N, PIN);

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

  pixels.Begin();
  pixels.Show();

}

void loop() {
  for (int i=0; i < PIXEL_N; i++) {
    Serial.printf("loop> i=%d\n", i);
    pixels.SetPixelColor(i, RgbColor(Col[Col_i][0],
                                     Col[Col_i][1],
                                     Col[Col_i][2]));
    pixels.Show();

    Col_i = (Col_i + 1) % COL_N;
    delay(50);
  }
}
