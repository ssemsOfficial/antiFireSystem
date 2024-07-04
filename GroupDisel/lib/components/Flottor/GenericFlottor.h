#ifndef GENERICFLOTTOR_H
#define GENERICFLOTTOR_H


class GenericFlottor
{
private:
    int _pin;
    bool _flottorState;
public:
    GenericFlottor(int pin)
        : _pin(pin)
    {}    
    ~GenericFlottor()
    {}
    
    void init(){
        pinMode(_pin, INPUT);
        _flottorState = digitalRead(_pin);
    }

    bool isActivated(){
        _flottorState = digitalRead(_pin);
        return _flottorState;
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    void setFlottorState(bool flottorState){
        _flottorState = flottorState;
    }
    bool getFlottorState(){
        return _flottorState;
    }
};

#endif