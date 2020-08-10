#include "HV5812.h"


// numDevice: no. of HV5812 connected in series
void HV5812::init(uint8_t numDevices, uint8_t pinClk, uint8_t pinData, uint8_t pinStrobe, uint8_t pinBlank){
    _numDevices = numDevices;
    _numPins = _numDevices * 20;
    _pinClk = pinClk;
    _pinData = pinData;
    _pinStrobe = pinStrobe;
    _pinBlank = pinBlank;

    Serial.println("pins: ");
    Serial.print("_pinClk: "); Serial.println(_pinClk);
    Serial.print("_pinData: "); Serial.println(_pinData);
    Serial.print("_pinStrobe: "); Serial.println(_pinStrobe);
    Serial.print("_pinBlank: "); Serial.println(_pinBlank);

    _pinsArray = new bool[_numPins];

    uint8_t outPins[4];
    outPins[0] = _pinClk;
    outPins[1] = _pinData;
    outPins[2] = _pinStrobe;
    outPins[3] = _pinBlank;
    for(int i = 0; i < 4; i++){
        pinMode(outPins[i], OUTPUT);
        digitalWrite(outPins[i], LOW);
    }
}

void HV5812::blank(){
    digitalWrite(_pinBlank, HIGH);
}

void HV5812::enable(){
    digitalWrite(_pinBlank, LOW);
}

void HV5812::flush(){
    digitalWrite(_pinData, LOW);

    for (int i = 0; i < _numPins; i++)
    {
        digitalWrite(_pinClk, LOW);
        delayClk();
        digitalWrite(_pinClk, HIGH);
        delayClk();

    }

    digitalWrite(_pinStrobe, HIGH);
    delayClk();
    digitalWrite(_pinStrobe, LOW);
    delayClk();
}

void HV5812::delayClk(){
    delayMicroseconds(CLK_DELAY_US);
}

void HV5812::write(){
    for (int i = 0; i < _numPins; i++)
    {
        // Serial.print(_pinsArray[i]);
        // Serial.print(" ");
        digitalWrite(_pinData, _pinsArray[i]);
        digitalWrite(_pinClk, HIGH);
        delayClk();
        digitalWrite(_pinClk, LOW);
        delayClk();
    }

    digitalWrite(_pinStrobe, HIGH);
    delayClk();
    digitalWrite(_pinStrobe, LOW);
    delayClk();
    // Serial.println("");
}

// pin: virtually assigned pin# out of total pin counts
//      1 - 19: closest device to MCU
//      20 - 39: second closest device to MCU
//      40 - ...
// value: HIGH or LOW
void HV5812::setPin(uint16_t pin, bool value){
    if(pin >= _numPins) return;

    _pinsArray[pin] = value;
}

uint16_t HV5812::getNumPins(){
    return _numPins;
}
