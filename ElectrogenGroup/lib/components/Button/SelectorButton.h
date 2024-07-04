#ifndef SELECTORBUTTON_H
#define SELECTORBUTTON_H

#include "GenericButton.h"


class SelectorButton : public GenericButton
{
private:
    
public:
    SelectorButton(int pin)
     :GenericButton(pin)
    {}    
    ~SelectorButton()
    {}
    bool isAutoModeSelected(){
        if(isPressed()) 
            return ON_BUTTON_AUTO_MODE_SELECTED;
        else
            return ON_BUTTON_MAN_MODE_SELECTED;
    }
};



#endif
