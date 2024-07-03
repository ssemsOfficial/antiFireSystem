#ifndef OILRESERVESYSTEM_H
#define OILRESERVESYSTEM_H

#include "../../BSP.h"

//components
#include "../../components/Relay/PompeRelay.h"
#include "../../components/Relay/MotorGroupRelay.h"
#include "../../components/Flottor/GenericFlottor.h"
#include "../../components/Button/GenericButton.h"
//adapters
#include"../../handlers/displayAdapter/DisplayAdapter.h"
//subSystem
#include"../alertSystem/alertSystem.h"



class OilReserveSystem
{
 private:
    //components
    GenericFlottor &flottor;
    PompeRelay &pump;
    bool* motor;
    GenericButton &testButton;
    //adapters
    DisplayAdapter &displayAdapter;
    //subSystem
    AlertSystem &alertSystem;    
    //events
    struct systemEvents 
    {
        bool OnReserve = false;
        bool OnTestBtn = false;
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
        bool pump = false;
        bool motor = false;
    };
    systemActions actions;

    bool mode = ON_AUTO_MODE_SELECTED;
    

 public:
    OilReserveSystem(AlertSystem &alertSystem, DisplayAdapter &displayAdapter, GenericFlottor &flottor, GenericButton &testButton, PompeRelay &pump)
        : alertSystem(alertSystem), displayAdapter(displayAdapter), flottor(flottor), testButton(testButton), pump(pump) 
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
        if( testButton.isPressed() == ON_TEST_BUTTON_CLICKED){
            events.OnTestBtn = ON_TEST_BUTTON_CLICKED;
        }
        else if (testButton.isPressed() == ON_TEST_BUTTON_RELEASED)
        {
            events.OnTestBtn = ON_TEST_BUTTON_RELEASED;
        }
}


void OilReserveSystem::FSM_oilReserveSystemAutoMode(){
    switch (states)
    {
        case INIT :
            if(events.OnReserve == ON_RESERVE_EMPTIED){
                states = RESERVE_IS_EMPTY;
                actions.pump = TURN_ON_PUMP;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if(events.OnReserve == ON_RESERVE_FILLED){
                states = RESERVE_IS_FULL;
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.pump = TURN_OFF_PUMP;
                break;
            }
        break;

        case RESERVE_IS_FULL :
            if (events.OnReserve == ON_RESERVE_EMPTIED)
            {
                actions.pump = TURN_ON_PUMP;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = RESERVE_IS_EMPTY;
            }
        break;
        
        case RESERVE_IS_EMPTY :
            if (events.OnReserve == ON_RESERVE_FILLED)
            {
                actions.pump = TURN_OFF_PUMP;
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
                actions.pump = TURN_ON_PUMP;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.OnReserve == ON_RESERVE_FILLED)
            {   
                states = RESERVE_IS_FULL;
                actions.pump = TURN_OFF_PUMP;
                if(events.OnTestBtn == ON_TEST_BUTTON_CLICKED){
                    actions.motor = TURN_ON_MOTOR_GROUP;
                    break;
                }
                else if (events.OnTestBtn == ON_TEST_BUTTON_RELEASED){
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                    break;
                }
            }
        break;

        case RESERVE_IS_FULL :
            if (events.OnReserve == ON_RESERVE_EMPTIED)
            {
                actions.pump = TURN_ON_PUMP;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = RESERVE_IS_EMPTY;
            }
            else
            {
                if(events.OnTestBtn == ON_TEST_BUTTON_CLICKED){
                    actions.motor = TURN_ON_MOTOR_GROUP;
                    break;
                }
                else if (events.OnTestBtn == ON_TEST_BUTTON_RELEASED){
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                    break;
                }
            }
        break;
        
        case RESERVE_IS_EMPTY :
            if (events.OnReserve == ON_RESERVE_FILLED)
            {
                states = RESERVE_IS_FULL;
                actions.pump = TURN_OFF_PUMP;
                if(events.OnTestBtn == ON_TEST_BUTTON_CLICKED){
                    actions.motor = TURN_ON_MOTOR_GROUP;
                    break;
                }
                else if (events.OnTestBtn == ON_TEST_BUTTON_RELEASED){
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                    break;
                }
            }
        break;
    }
}


void OilReserveSystem::updateActions(){
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
    updateMotorAction(actions.motor);    
}

#endif