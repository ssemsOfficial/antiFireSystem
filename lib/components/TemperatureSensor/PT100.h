#ifndef PT100_H
#define PT100_H

#include "GenericTemperatureSensor.h"

class PT100 : public GenericTemperatureSensor
{
private:
    float _temperatureValue;
public:
    PT100(int pin)
        :GenericTemperatureSensor(pin)
    {}
    ~PT100()
    {}
    float getTemperatureValue(){
        float _temperatureValue = map(getTempSensorValue(), 0, 1023, -50, 850);
        return _temperatureValue;
    }
    int getTempSensorValue(){
        _tempSensorValue = analogRead(_pin);
        return _tempSensorValue;
    }
};




#endif