#ifndef GENERICBUZZER_H
#define GENERICBUZZER_H

#include"../BSP.h"

class GenericBuzzer
{
private:
    int _pin;
    int _buzzerState;

public:
    GenericBuzzer(int pin)
        : _pin(pin)
    {}
    ~GenericBuzzer()
    {}

    void init(){
        pinMode(_pin, OUTPUT);
        _buzzerState = BUZZER_DESACTIVATED;
    }
    void activateBuzzer(){
        digitalWrite(_pin, ACTIVATE_BUZZER);
        _buzzerState = BUZZER_ACTIVATED;
    }
    void desactivateBuzzer(){
        digitalWrite(_pin, DESACTIVATE_BUZZER);
        _buzzerState = BUZZER_DESACTIVATED;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setBuzzerState(bool buzzerState){
        _buzzerState = buzzerState;
    }
    bool getBuzzerState(){
        return _buzzerState;
    }
};


#endif