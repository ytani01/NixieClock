/*
 *
 */
#include "NixieArray.h"

#define PIN_INTR 2
#define DEBOUNCE 100

#define PIN_HV5812_CLK 26
#define PIN_HV5812_STROBE 13
#define PIN_HV5812_DATA 14
#define PIN_HV5812_BLANK 4
#define PIN_COLON_R 16
#define PIN_COLON_L 17
#define PIN_LED 27

#define PIN_SW1 33
#define PIN_SW2 34
#define PIN_SW3 35

#define VERSION_MODE	0x01
#define CLOCK_MODE	0x10
#define TEST1_MODE	0xF1

#define LOOP_DELAY	0  // msec
//============================================================================
uint8_t pins_in[]  = {PIN_SW1, PIN_SW2, PIN_SW3};

uint8_t nixie_pins[NIXIE_N][DIGIT_N] =
  {{ 9,  0,  1,  2,  3,  4,  5,  6,  7,  8},
   {19, 10, 11, 12, 13, 14, 15, 16, 17, 18},
   {29, 20, 21, 22, 23, 24, 25, 26, 27, 28},
   {39, 30, 31, 32, 33, 34, 35, 36, 37, 38},
   {49, 40, 41, 42, 43, 44, 45, 46, 47, 48},
   {59, 50, 51, 52, 53, 54, 55, 56, 57, 58} };

uint8_t colon_pins[] = {PIN_COLON_R, PIN_COLON_L};

NixieArray nixieArray;

nixie_disp_data_t nixieDispData;

unsigned long	loopCount	= 0;
unsigned long	curMsec		= 0;  // msec
uint8_t		curNixie	= 0;

uint8_t		curMode		= 0x00;
//============================================================================
void btn_handler() {
  static unsigned long prev_msec = 0;
  unsigned long cur_msec = millis();

  if ( cur_msec - prev_msec < DEBOUNCE ) {
    return;
  }

  prev_msec = cur_msec;

  if ( digitalRead(PIN_SW1) == LOW ) {
    Serial.println("curMsec="+String(curMsec));
    
    for (int nx=0; nx < NIXIE_N; nx++) {
      Serial.println(" nx=" + String(nx));

      Nixie n1 = nixieArray.get_nixie(nx);
      for (int d=0; d < DIGIT_N; d++) {
	uint8_t bl = n1.get_blightness(d);
	Serial.print(" " + String(bl));
      }

      Serial.println();
    } // for(nx)
  }

  if ( digitalRead(PIN_SW2) == LOW) {
    curNixie = (curNixie + 1) % NIXIE_N;
    Serial.println("curNixie=" + String(curNixie));
  }

  if ( digitalRead(PIN_SW3) == LOW ) {
    nixieDispData.nx_blightness[0][0]--;
    nixieDispData.nx_blightness[0][1]++;
  }
} // btn_handler
//============================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("begin");

  nixieArray.init(PIN_HV5812_CLK,  PIN_HV5812_STROBE,
		  PIN_HV5812_DATA, PIN_HV5812_BLANK,
		  nixie_pins, colon_pins, PIN_LED);

  memset(nixieDispData.nx_blightness, 0, sizeof(uint8_t) * NIXIE_N * DIGIT_N);
  for (int nx=0; nx < NIXIE_N; nx++) {
    for (int d=0; d < DIGIT_N; d++) {
      if ( nx == d ) {
	nixieDispData.nx_blightness[nx][d] = BLIGHTNESS_MAX;
      }
    } // for(d)
  } // for(nx)

  nixieDispData.colon[COLON_R]	= HIGH;
  nixieDispData.colon[COLON_L]	= HIGH;
  nixieDispData.led		= LOW;

  for (int i=0; i < sizeof(pins_in) / sizeof(uint8_t); i++) {
    pinMode(pins_in[i], INPUT);

    int val = digitalRead(pins_in[i]);
    Serial.println("SW[" + String(i) + "]=" + String(val) );
  }

  //--------------------------------------------------------------------------
  attachInterrupt(digitalPinToInterrupt(PIN_SW1), btn_handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SW2), btn_handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SW3), btn_handler, CHANGE);
  //--------------------------------------------------------------------------
  //versionMode.init();
  //clockMode.init();
  //test1Mode.init();
} // setup
//============================================================================
void loop() {
  curMsec = millis();

  //--------------------------------------------------------------------------
  switch ( curMode ) {
    //case VERSION_MODE: versionMode.display(curMsec, nixieDigit); break;
    //case CLOCK_MODE:   clockMode.display(curMsec, nixieDigit);   break;
    //case TEST1_MODE:   test1Mode.display(curMsec, nixieDigit);   break;
  default: break;
  } // switch
  //--------------------------------------------------------------------------
  nixieArray.display(curMsec, &nixieDispData);

  loopCount++;
  delay(LOOP_DELAY);
} // loop()
//============================================================================