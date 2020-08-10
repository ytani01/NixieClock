#include "Tubes.h"

void Digit::setPinsArray(uint8_t *pins){
    for(int i=0; i<PINS_PER_DIGIT; i++) _pins[i] = pins[i]; 
}

uint8_t* Digit::getPinsArray(){
    return _pins;
}

void Digit::enable(){
    _enabled = true;
}

void Digit::disbable(){
    _enabled = false;
}

bool Digit::isEnabled(){
    return _enabled;
}

void Digit::setValue(uint8_t value){
    _value = value;
}

uint8_t Digit::getValue(){
    return _value;
}

void Tubes::init(TubesInitObject obj){
    init(obj.numDevices, obj.pinClk, obj.pinData, obj.pinStrobe, obj.pinBlank);
    initDigits(obj.numDigits);
    initColon(obj.pinColonR, obj.pinColonL);
}

void Tubes::init(uint8_t numDevices, uint8_t pinClk, uint8_t pinData, uint8_t pinStrobe, uint8_t pinBlank){
    nix.init(numDevices, pinClk, pinData, pinStrobe, pinBlank);
    nix.flush();
    nix.enable();
}

void Tubes::initDigits(uint8_t numDigits){
    _numDigits = numDigits;
    digits = new Digit[_numDigits];
}

void Tubes::initColon(uint8_t pinColonR, uint8_t pinColonL){
    _pinColonR = pinColonR;
    _pinColonL = pinColonL;
    pinMode(_pinColonL, OUTPUT);
    pinMode(_pinColonR, OUTPUT);
    digitalWrite(_pinColonR, LOW);
    digitalWrite(_pinColonL, LOW);
}

// arg: uint8_t *pins: pin# of each cathodes
//      uint8_t n: nth digit
void Tubes::addDigit(uint8_t n, uint8_t *pins){
    digits[n].setPinsArray(pins);
    digits[n].setValue(0);
    digits[n].disbable();
}

uint8_t Tubes::numDigits(){
    return _numDigits;
}

void Tubes::setNum(uint8_t digit, uint8_t num){
    digits[digit].setValue(num);
}

void Tubes::show(){
    for (int i = 0; i < _numDigits; i++)
    {
        uint8_t *pins = digits[i].getPinsArray();
        for (int j = 0; j < PINS_PER_DIGIT; j++) nix.setPin(pins[j], HIGH);
        if(digits[i].isEnabled()){
            
            uint8_t val = digits[i].getValue();
            nix.setPin(pins[val], LOW);
        }
        // else{
        //     for (int j = 0; j < PINS_PER_DIGIT; j++) nix.setPin(pins[j], HIGH);
        // }
    }

    nix.write();
}

void Tubes::clear(){
    for (int i = 0; i < nix.getNumPins(); i++)
    {
        nix.setPin(i, HIGH);
    }
}

void Tubes::setTransitionMode(uint8_t tmode){

}

void Tubes::setValue(unsigned long value){
    int pow = 1;

    for (int i = 0; i < _numDigits; i++)
    {
        int n = value / pow;
        digits[i].setValue(n % 10);
        pow *= 10;
    }
}

// Disable single digit
void Tubes::enableDigit(uint8_t digit){
    digits[digit].enable();
}

// enable single digit 
void Tubes::disableDigit(uint8_t digit){
    digits[digit].disbable();
}

void Tubes::enableAll(){
    for (int i = 0; i < _numDigits; i++) digits[i].enable();

}

void Tubes::disableAll(){
    for (int i = 0; i < _numDigits; i++) digits[i].disbable();
}

void Tubes::update(){

}

void Tubes::blank(){
    nix.blank();
}

void Tubes::deassertBlank(){
    nix.enable();
}