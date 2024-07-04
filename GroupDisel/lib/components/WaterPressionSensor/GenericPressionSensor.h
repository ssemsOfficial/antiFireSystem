#ifndef GENERICPRESSIONSENSOR_H
#define GENERICPRESSIONSENSOR_H

#include<Arduino.h>

class GenericPressionSensor
{
private:
    int _pin;
    int _pressionSensorValue;
public:
    GenericPressionSensor(int pin)
        : _pin(pin)
    {}    
    ~GenericPressionSensor()
    {}
    
    void init(){
        pinMode(_pin, INPUT);
    }

    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    int getPressionSensorValue(){
        _pressionSensorValue = analogRead(_pin);
        return _pressionSensorValue;
    }
    float getPressionValue(){
        float _temperatureValue = map(getPressionSensorValue(), 0, 1023, 0, 1000);
        return _temperatureValue;
    }
};

#endif