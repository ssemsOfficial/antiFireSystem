#ifndef SELECTBATPOSUPRELAY_H
#define SELECTBATPOSUPRELAY_H

#include "GenericRelay.h"

class SelectBatPoSupRelay : public GenericRelay
{
private:
    
public:
    SelectBatPoSupRelay(int pin)
        :GenericRelay(pin)
    {}    
    ~SelectBatPoSupRelay()
    {}
    void selectToChargeBat1(){
        openRelay();
    }
    void selectToChargeBat2(){
        closeRelay();
    }
};

#endif
