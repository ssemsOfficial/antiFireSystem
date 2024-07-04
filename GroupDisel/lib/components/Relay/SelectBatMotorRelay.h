#ifndef SELECTBATMOTORRELAY_H
#define SELECTBATMOTORRELAY_H

#include "GenericRelay.h"

class SelectBatMotorRelay : public GenericRelay
{
private:
    
public:
    SelectBatMotorRelay(int pin)
        :GenericRelay(pin)
    {}    
    ~SelectBatMotorRelay()
    {}
    void selectToUseBat1(){
        openRelay();
    }
    void selectToUseBat2(){
        closeRelay();
    }
};

#endif
