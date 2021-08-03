#include <Adafruit_NeoPixel.h>

const uint8_t PIN_PIXEL  = 16;
const uint8_t PIXELS_N   = 6;
const int     LOOP_DELAY = 500;  // ms

Adafruit_NeoPixel pixels(PIXELS_N, PIN_PIXEL, NEO_GRB + NEO_KHZ800);

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

  pixels.begin();
  digitalWrite(5, HIGH);

  pixels.clear();
}

void loop() {
  for (int i=0; i < PIXELS_N; i++) {
    Serial.printf("loop> i=%d\n", i);
    pixels.setPixelColor(i, pixels.Color(Col[Col_i][0],
                                         Col[Col_i][1],
                                         Col[Col_i][2]));
    pixels.show();

    Col_i = (Col_i + 1) % COL_N;
    delay(LOOP_DELAY);
  }
}
