#include <Adafruit_NeoPixel.h>

#define PIN   2
#define NUMPIXELS 6

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

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
  for (int i=0; i < NUMPIXELS; i++) {
    Serial.printf("loop> i=%d\n", i);
    pixels.setPixelColor(i, pixels.Color(Col[Col_i][0],
                                         Col[Col_i][1],
                                         Col[Col_i][2]));
    pixels.show();

    Col_i = (Col_i + 1) % COL_N;
    delay(500);
  }
}
