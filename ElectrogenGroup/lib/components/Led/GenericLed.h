#ifndef GENERIClED_H
#define GENERIClED_H

#include"../BSP.h"

class GenericLed
{
private:
    int _pin;
    bool _ledState;
public:
    GenericLed(int pin)
        : _pin(pin)
    {}    
    ~GenericLed()
    {}

    void init(){
        pinMode(_pin, OUTPUT);
        _ledState = digitalRead(_pin);
    }
    void turnOnLed(){
        digitalWrite(_pin, TURN_ON_LED);
        _ledState = LED_IS_TURNED_ON;
    }
    void turnOffLed(){
        digitalWrite(_pin, TURN_OFF_LED);
        _ledState = LED_IS_TURNED_OFF;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setLedState(bool ledState){
        _ledState = ledState;
    }
    bool getLedState(){
        return _ledState;
    }
};




#endif