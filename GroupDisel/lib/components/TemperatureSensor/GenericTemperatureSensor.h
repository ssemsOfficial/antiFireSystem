#ifndef GENERICTEMPERATURESENSOR_H
#define GENERICTEMPERATURESENSOR_H



class GenericTemperatureSensor
{
protected:
    int _pin;
    int _tempSensorValue;
public:
    GenericTemperatureSensor(int pin)
        : _pin(pin)
    {}    
    ~GenericTemperatureSensor()
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
};

#endif