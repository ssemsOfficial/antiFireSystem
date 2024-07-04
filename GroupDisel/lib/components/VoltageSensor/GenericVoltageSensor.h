#ifndef GENERICVOLTAGESENSOR_H
#define GENERICVOLTAGESENSOR_H



class GenericVoltageSensor
{
protected:
    int _pin;
    int _sensorValue;
public:
    GenericVoltageSensor(int pin)
        : _pin(pin)
    {}    
    ~GenericVoltageSensor()
    {}
    
    void init(){
        pinMode(_pin, INPUT);
        _sensorValue = getSensorValue();
    }
    void setPin(int pin){
        _pin = pin;
    }
    int getPin(){
        return _pin;
    }
    int getSensorValue(){
        _sensorValue = analogRead(_pin);
        return _sensorValue;
    }
};

#endif