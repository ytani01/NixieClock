/*
 * NixieTubeClock.ino
 */
#include "NixieArray.h"
#include "CmdQueue.h"
#include "ModeBase.h"
#include "ModeTest1.h"

#define PIN_INTR           2 // ??
#define DEBOUNCE         200 // msec

#define PIN_HV5812_CLK    26
#define PIN_HV5812_STOBE  13
#define PIN_HV5812_DATA   14
#define PIN_HV5812_BLANK   4

#define PIN_COLON_R       16
#define PIN_COLON_L       17
#define PIN_LED           27

#define PIN_SW1           33
#define PIN_SW2           34
#define PIN_SW3           35

#define LOOP_DELAY         0 // msec
//============================================================================
uint8_t pinsIn[] = {PIN_SW1, PIN_SW2, PIN_SW3};

uint8_t nixiePins[NIXIE_NUM_N][NIXIE_NUM_DIGIT_N] =
  {{ 9,  0,  1,  2,  3,  4,  5,  6,  7,  8},
   {19, 10, 11, 12, 13, 14, 15, 16, 17, 18},
   {29, 20, 21, 22, 23, 24, 25, 26, 27, 28},
   {39, 30, 31, 32, 33, 34, 35, 36, 37, 38},
   {49, 40, 41, 42, 43, 44, 45, 46, 47, 48},
   {59, 50, 51, 52, 53, 54, 55, 56, 57, 58} };

uint8_t colonPins[] = {PIN_COLON_R, PIN_COLON_L};

NixieArray nixieArray;
CmdQueue cmdQ;

unsigned long loopCount  = 0;
unsigned long curMsec    = 0; // msec
unsigned long prevMsec   = 0;

int curTube = 0;
int curDigit = 0;

//----------------------------------------------------------------------------
#define MODE_TEST1 0
ModeTest1 modeT1;

ModeBase *Mode[] = {&modeT1};

static unsigned long ModeN = sizeof(Mode) / sizeof(ModeBase *);

unsigned long curMode = MODE_TEST1;
//============================================================================
void btn_handler() {
  static unsigned long prev_msec = 0;
  unsigned long cur_msec = millis();

  if ( cur_msec - prev_msec < DEBOUNCE ) {
    return;
  }
  prev_msec = cur_msec;

  if ( digitalRead(PIN_SW1) == LOW ) {
    curDigit--;
    if ( curDigit < 0 ) {
      curDigit = NIXIE_NUM_DIGIT_N - 1;
      curTube--;
      if ( curTube < 0 ) {
	curTube = NIXIE_NUM_N - 1;
      }
    }
    
    Serial.print("curTube:" + String(curTube) + " ");
    Serial.print("curDigit:" + String(curDigit) + " ");
    Serial.println();
  }

  if ( digitalRead(PIN_SW2) == LOW) {
    Serial.print("curTube:" + String(curTube) + " ");
    Serial.print("curDigit:" + String(curDigit) + " ");

    uint8_t bl = nixieArray.get_num_blightness(curTube, curDigit);
    bl++;
    if ( bl > BLIGHTNESS_MAX ) {
      bl=0;
    }
    nixieArray.set_num_blightness(curTube, curDigit, bl);
    Serial.print("bl=" + String(bl));
    Serial.println();
  }

  if ( digitalRead(PIN_SW3) == LOW ) {
    curDigit++;
    if ( curDigit >= NIXIE_NUM_DIGIT_N ) {
      curDigit = 0;
      curTube++;
      if ( curTube >= NIXIE_NUM_N ) {
	curTube = 0;
      }
    }
    
    Serial.print("curTube:" + String(curTube) + " ");
    Serial.print("curDigit:" + String(curDigit) + " ");
    Serial.println();
  }
} // btn_handler
//============================================================================
void inc_nixie() {
  static uint8_t cur_num = NIXIE_NUM_N - 1;
  static uint8_t cur_dg = NIXIE_NUM_DIGIT_N - 1;
  static uint8_t cur_bl = 0;

  cur_bl++;
  if ( cur_bl > BLIGHTNESS_MAX ) {
    cur_bl = 0;
    nixieArray.set_num_blightness(cur_num, cur_dg, 0); 
    cur_dg++;
    if ( cur_dg >= NIXIE_NUM_DIGIT_N ) {
      cur_dg = 0;
      cur_num++;
      if ( cur_num >= NIXIE_NUM_N ) {
	cur_num=0;
      }
    }
  }

  uint8_t prev_nx = cur_num;
  uint8_t prev_dg = cur_dg - 1;

  if ( prev_dg >= NIXIE_NUM_DIGIT_N ) {
    prev_dg = NIXIE_NUM_DIGIT_N - 1;
    prev_nx--;
    if ( prev_nx >= NIXIE_NUM_N ) {
      prev_nx = NIXIE_NUM_N - 1;
    }
  }

  nixieArray.set_num_blightness(prev_nx, prev_dg,
				     BLIGHTNESS_MAX - cur_bl); 
  nixieArray.set_num_blightness(cur_num, cur_dg, cur_bl); 
}

void timer50ms(unsigned long msec) {
  //inc_nixie();
}

void timer100ms(unsigned long msec) {
  inc_nixie();
}

void timer1sec(unsigned long msec) {
  Serial.println("msec=" + String(msec));

  unsigned long sec = msec / 1000;

  if ( sec % 2 == 0 ) {
    nixieArray.set_colon_blightness(COLON_L, BLIGHTNESS_MAX);
    nixieArray.set_colon_blightness(COLON_R, 0);
  } else {
    nixieArray.set_colon_blightness(COLON_L, 0);
    nixieArray.set_colon_blightness(COLON_R, BLIGHTNESS_MAX);
  }
}
//============================================================================
void setup() {
  Serial.begin(115200);
  Serial.println("begin");
  //--------------------------------------------------------------------------
  nixieArray.init(PIN_HV5812_CLK, PIN_HV5812_STOBE,
                      PIN_HV5812_DATA, PIN_HV5812_BLANK,
                      nixiePins, colonPins);
  cmdQ.init(&nixieArray);
  //--------------------------------------------------------------------------
  for (int m=0; m < ModeN; m++) {
    Mode[m]->setup(m, &nixieArray, &cmdQ);
  }
  //--------------------------------------------------------------------------
  for (int i=0; i < sizeof(pinsIn) / sizeof(uint8_t); i++) {
    pinMode(pinsIn[i], INPUT);
    int val = digitalRead(pinsIn[i]);
    Serial.println("SW[" + String(i) + "]=" + String(val) );
  }
  Serial.println("digitalPinToInterrupt:");
  Serial.println(" " + String(PIN_SW1) + ":" +
                 String(digitalPinToInterrupt(PIN_SW1)));
  Serial.println(" " + String(PIN_SW2) + ":" +
                 String(digitalPinToInterrupt(PIN_SW2)));
  Serial.println(" " + String(PIN_SW3) + ":" +
                 String(digitalPinToInterrupt(PIN_SW3)));
  attachInterrupt(digitalPinToInterrupt(PIN_SW1), btn_handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SW2), btn_handler, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_SW3), btn_handler, CHANGE);
} // setup()
//============================================================================
void loop() {
  prevMsec = curMsec;
  curMsec = millis();

  if ( (curMsec / 1000) != (prevMsec / 1000) ) {
    timer1sec(curMsec);
  }
  if ( (curMsec / 100) != (prevMsec / 100) ) {
    timer100ms(curMsec);
  }
  if ( (curMsec / 50) != (prevMsec / 50) ) {
    timer50ms(curMsec);
  }

  Mode[curMode]->loop(curMsec);
  
  cmdQ.loop();
  nixieArray.display(curMsec);

  loopCount++;
  delay(LOOP_DELAY);
} // loop()