#ifndef MOTORGROUPRELAY_H
#define MOTORGROUPRELAY_H

#include "GenericRelay.h"

class MotorGroupRelay : public GenericRelay
{
private:
    
public:
    MotorGroupRelay(int pin)
        :GenericRelay(pin)
    {}    
    ~MotorGroupRelay()
    {}
    
    void arreterMotorGroup(){
        openRelay();
    }
    void demarrerMotorGroup(){
        closeRelay();
    }
};

#endif
