#ifndef BATTERYPOWERSUPPLYRELAY_H
#define BATTERYPOWERSUPPLYRELAY_H

#include "GenericRelay.h"

class BatteryPowerSupplyRelay : public GenericRelay
{
private:
    
public:
    BatteryPowerSupplyRelay(int pin)
        :GenericRelay(pin)
    {}    
    ~BatteryPowerSupplyRelay()
    {}
    
    void cutOffPowerSupply(){
        openRelay();
    }
    void turnOnPowerSupply(){
        closeRelay();
    }
};

#endif
