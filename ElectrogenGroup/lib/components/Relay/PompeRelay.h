#ifndef POMPERELAY_H
#define POMPERELAY_H

#include "GenericRelay.h"


class PompeRelay : public GenericRelay
{
private:
    
public:
    PompeRelay(int pin)
        :GenericRelay(pin)
    {

    }    
    ~PompeRelay()
    {}

    void arreterPompe(){
        openRelay();
    }
    void demarrerPompe(){
        closeRelay();
    }
};



#endif
