#ifndef PNEUMATICDISTRIBUTORRELAY_H
#define PNEUMATICDISTRIBUTORRELAY_H

#include "GenericRelay.h"

class PneumaticDistributorRelay : public GenericRelay
{
private:
    
public:
    PneumaticDistributorRelay(int pin)
        :GenericRelay(pin)
    {}    
    ~PneumaticDistributorRelay()
    {}
    
    void turnOff(){
        openRelay();
    }
    void TurnOn(){
        closeRelay();
    }
};

#endif
