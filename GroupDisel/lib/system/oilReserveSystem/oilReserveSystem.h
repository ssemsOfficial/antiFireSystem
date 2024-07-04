#ifndef OILRESERVESYSTEM_H
#define OILRESERVESYSTEM_H

#include "../../BSP.h"

//components
//#include "../../components/Relay/PompeRelay.h"
#include "../../components/Relay/MotorGroupRelay.h"
#include "../../components/Flottor/GenericFlottor.h"
#include "../../components/Button/GenericButton.h"

//adapters
#include"../handler/displayAdapter/DisplayAdapter.h"

//subSystem
#include"../alertSystem/alertSystem.h"



class OilReserveSystem
{
 private:
    //components
    GenericFlottor &flottor;
     //PompeRelay &pump;
    bool* motor;
    GenericButton &motorTurnOnBtn;
    GenericButton &motorTurnOffBtn;

    //adapters
    DisplayAdapter &displayAdapter;
    //subSystem
    AlertSystem &alertSystem;    
    //events
    struct systemEvents 
    {
        bool OnReserve = false;
        bool OnMotorTurnOnBtn = false;
        bool OnMotorTurnOffBtn = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        RESERVE_IS_FULL,
        RESERVE_IS_EMPTY
    };
    systemStates states = INIT;
    
    //actions
    struct systemActions
    {
        //bool pump = false;
        bool motor = false;
        bool display = false;
    };
    systemActions actions;

    bool mode = ON_AUTO_MODE_SELECTED;
    

 public:
    OilReserveSystem(DisplayAdapter &displayAdapter, GenericFlottor &flottor, GenericButton &motorTurnOnBtn, GenericButton &motorTurnOffBtn,/*, PompeRelay &pump*/ AlertSystem &alertSystem)
        : displayAdapter(displayAdapter), flottor(flottor), motorTurnOnBtn(motorTurnOnBtn), motorTurnOffBtn(motorTurnOnBtn),/*, pump(pump)*/ alertSystem(alertSystem)
    {
        motor = nullptr;
    }
    ~OilReserveSystem()
    {
        delete motor;
    }

    void updateEvents();   
    void FSM_oilReserveSystemAutoMode();
    void FSM_oilReserveSystemManMode();
    void updateActions();
    void setMode(bool mode){
        this->mode = mode; 
    }
    void setMotor(bool &globalMotor){
        motor = &globalMotor;
    }
    void updateMotorAction(int action){
        *motor = (bool)action;
    }
};


void OilReserveSystem::updateEvents(){
        if ( flottor.isActivated()  == ON_FLOTTOR_ACTIVATED)
        {
            events.OnReserve = ON_RESERVE_EMPTIED;
            alertSystem.updateOnReserveEvent(ON_RESERVE_EMPTIED);
        }
        else if(flottor.isActivated()  == ON_FLOTTOR_DESACTIVATED )
        {
            events.OnReserve = ON_RESERVE_FILLED;
            alertSystem.updateOnReserveEvent(ON_RESERVE_FILLED);
        }

        if( motorTurnOnBtn.isPressed() == ON_TEST_BUTTON_CLICKED ){
           events.OnMotorTurnOnBtn = ON_TEST_BUTTON_CLICKED;
        }
        else if (motorTurnOnBtn.isPressed() == ON_TEST_BUTTON_RELEASED)
        {
           events.OnMotorTurnOnBtn = ON_TEST_BUTTON_RELEASED; 
        } 

        if( motorTurnOnBtn.isPressed() == ON_TEST_BUTTON_CLICKED ){
           events.OnMotorTurnOffBtn = ON_TEST_BUTTON_CLICKED;
        }
        else if (motorTurnOnBtn.isPressed() == ON_TEST_BUTTON_RELEASED)
        {
           events.OnMotorTurnOffBtn = ON_TEST_BUTTON_RELEASED; 
        } 
}


void OilReserveSystem::FSM_oilReserveSystemAutoMode(){
    switch (states)
    {
        case INIT :
            if(events.OnReserve == ON_RESERVE_EMPTIED){
                states = RESERVE_IS_EMPTY;
                //actions.pump = TURN_ON_PUMP;
                actions.display = DISPLAY_RESERVE_EMPTY;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if(events.OnReserve == ON_RESERVE_FILLED){
                states = RESERVE_IS_FULL;
                //actions.pump = TURN_OFF_PUMP;
                actions.display = DISPLAY_RESERVE_FULL;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;
            }
        break;

        case RESERVE_IS_FULL :
            if (events.OnReserve == ON_RESERVE_EMPTIED)
            {
                //actions.pump = TURN_ON_PUMP;
                actions.display = DISPLAY_RESERVE_FULL;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = RESERVE_IS_EMPTY;
            }
        break;
        
        case RESERVE_IS_EMPTY :
            if (events.OnReserve == ON_RESERVE_FILLED)
            {
                //actions.pump = TURN_OFF_PUMP;
                actions.display = DISPLAY_RESERVE_EMPTY;
                actions.motor = TURN_ON_MOTOR_GROUP;
                states = RESERVE_IS_FULL;
            }
        break;
    }
}
void OilReserveSystem::FSM_oilReserveSystemManMode(){
    switch (states)
    {
        case INIT :
            if(events.OnReserve == ON_RESERVE_EMPTIED){
                states = RESERVE_IS_EMPTY;
                //actions.pump = TURN_ON_PUMP;
                actions.display = DISPLAY_RESERVE_EMPTY;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.OnReserve == ON_RESERVE_FILLED)
            {   
                states = RESERVE_IS_FULL;
                actions.display = DISPLAY_RESERVE_FULL;
                //actions.pump = TURN_OFF_PUMP;
                if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                }
                break;
            }
        break;

        case RESERVE_IS_FULL :
            if (events.OnReserve == ON_RESERVE_EMPTIED)
            {
                //actions.pump = TURN_ON_PUMP;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.display = DISPLAY_RESERVE_EMPTY;
                states = RESERVE_IS_EMPTY;
            }
            else
            {
                if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                }
                break;
            }
        break;
        
        case RESERVE_IS_EMPTY :
            if (events.OnReserve == ON_RESERVE_FILLED)
            {
                states = RESERVE_IS_FULL;
                //actions.pump = TURN_OFF_PUMP;
                actions.display = DISPLAY_RESERVE_FULL;
                if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                }
                break;
            }
        break;
    }
}


void OilReserveSystem::updateActions(){
    
    if (actions.display == DISPLAY_RESERVE_EMPTY)
    {
        displayAdapter.setReserveLevel("EMPTY");
    }
    else if (actions.display == DISPLAY_RESERVE_FULL)
    {
        displayAdapter.setReserveLevel("FULL");
    }
    updateMotorAction(actions.motor);    

    /*
    if (actions.pump == TURN_ON_PUMP)
    {
        displayAdapter.setReserveLevel("EMPTY");
        pump.demarrerPompe();
    }
    else if (actions.pump == TURN_OFF_PUMP)
    {
        displayAdapter.setReserveLevel("FULL");
        pump.arreterPompe();
    }
    */
}




#endif