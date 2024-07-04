#ifndef GENERICRELAY_H
#define GENERICRELAY_H

#include<Arduino.h>

#include"../BSP.h"

class GenericRelay
{
protected:
    int _pin;
    bool _state;
public:
    GenericRelay(int pin)
        : _pin(pin)
    {}    
    ~GenericRelay()
    {}
    
    void init(){
        pinMode(_pin, OUTPUT);
        _state = digitalRead(_pin);
    }
    void openRelay(){
        digitalWrite(_pin, OPEN_GENERIC_RELAY);
        _state = ON_GENERIC_RELAY_OPENED;
    }
    void closeRelay(){
        digitalWrite(_pin, CLOSE_GENERIC_RELAY);
        _state = ON_GENERIC_RELAY_CLOSED;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setRelayState(bool RelayState){
        _state = RelayState;
    }
    bool getRelayState(){
        return _state;
    }
};

#endif