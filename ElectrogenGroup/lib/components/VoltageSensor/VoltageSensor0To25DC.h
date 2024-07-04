#ifndef VOLTAGESENSOR0TO25DC_H
#define VOLTAGESENSOR0TO25DC_H

#include "GenericVoltageSensor.h"

class VoltageSensor0To25DC : public GenericVoltageSensor
{
private:
    float _voltageValue;
public:
    VoltageSensor0To25DC(int pin)
        :GenericVoltageSensor(pin)
    {}
    ~VoltageSensor0To25DC()
    {}
    float getVoltageValue(){
        getSensorValue(); 
        _voltageValue = ( _sensorValue * 2.4 / 491.0) / (7.5/37.5);
        return _voltageValue;
    }
    
    
};




#endif