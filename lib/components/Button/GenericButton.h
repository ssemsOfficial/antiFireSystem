#ifndef GENERICBUTTON_H
#define GENERICBUTTON_H

#include"../BSP.h"

class GenericButton
{
protected:
    int _pin;
    bool _buttonState; 
public:
    GenericButton(int pin)
        : _pin(pin)
    {}
    ~GenericButton()
    {}

    void init(){
        pinMode(_pin, INPUT);
        _buttonState = digitalRead(_pin);
    }
    bool isPressed(){
        _buttonState = digitalRead(_pin);
        return _buttonState;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setButtonState(bool buttonState){
        _buttonState = buttonState;
    }
    bool getButtonState(){
        return _buttonState;
    }

};

#endif