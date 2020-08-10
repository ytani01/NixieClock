#include <Arduino.h>
#include "HV5812.h"
#define PINS_PER_DIGIT 10

class TubesInitObject{
    public:
        uint8_t numDevices;
        uint8_t pinClk;
        uint8_t pinData;
        uint8_t pinStrobe;
        uint8_t pinBlank;
        uint8_t pinColonR;
        uint8_t pinColonL;
        uint8_t numDigits;
};

class Digit{
    public:
        void setPinsArray(uint8_t *pins);
        uint8_t* getPinsArray();
        void enable();
        void disbable();
        bool isEnabled();
        void setValue(uint8_t value);
        uint8_t getValue();
    
    private:
        uint8_t _pins[10];
        bool _enabled = true;
        uint8_t _value;
};

class Tubes{
    public:
        Tubes(){};
        void init(TubesInitObject obj);
        void init(uint8_t numDevices, uint8_t pinClk, uint8_t pinData,
		  uint8_t pinStrobe, uint8_t pinBlank); //HV5812 pins
        void initColon(uint8_t pinColonR, uint8_t pinColorL);
        void initDigits(uint8_t numDigits);
        void addDigit(uint8_t digit, uint8_t *pins);

        void blank();
        void deassertBlank();

        uint8_t numDigits();
        void show();
        void setNum(uint8_t digit, uint8_t num);
        void setValue(unsigned long value);
        void clear();
        void update();
        void setTransitionMode(uint8_t tmode);
        void step();
        
        void disableDigit(uint8_t digit);
        void enableDigit(uint8_t digit);
n        void enableAll();
        void disableAll();


    private:
        HV5812 nix;
        uint8_t _pinColonR, _pinColonL;
        Digit *digits; //array of pointers to cathode pin array
        uint8_t _numDigits;
};

