#include <Arduino.h>
#define CLK_DELAY_US 200
/*
#define HIGH 1
#define LOW 0
*/

class HV5812{
    public:
        HV5812(){};
        void init(uint8_t numDevices, uint8_t pinClk, uint8_t pinData, uint8_t pinStrobe, uint8_t pinBlank);
        void blank(); // write HIGH to blank pin
        void enable(); // write LOW to blank pin
        void flush(); // clear all registers with 0
        void write(); 
        void setPin(uint16_t pin, bool value);
        void getPin(uint16_t pin);
        uint16_t getNumPins();

    private:
        void delayClk();
        bool *_pinsArray;
        uint8_t _numDevices, _pinClk, _pinData, _pinStrobe, _pinBlank;
        uint16_t _numPins;
};

