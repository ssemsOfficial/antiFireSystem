#ifndef WATERPRESSIONSYSTEM_H
#define WATERPRESSIONSYSTEM_H

//components
#include "../../components/WaterPressionSensor/GenericPressionSensor.h"
#include "../../components/Relay/MotorGroupRelay.h"
#include "../../components/Button/GenericButton.h"
//adapters
#include "../handler/displayAdapter/DisplayAdapter.h"
//subSystem
#include "../alertSystem/alertSystem.h"

class WaterPressionSystem
{
 private:
    //components
    GenericPressionSensor &pressionSensor;
    bool* motor;
    GenericButton &motorTurnOnBtn;
    GenericButton &motorTurnOffBtn;

    //adapter
    DisplayAdapter &displayAdapter;
    //subSystem
    AlertSystem &alertSystem;
    //events
    struct systemEvents 
    {
        bool onWaterPression = false;
        bool OnMotorTurnOnBtn = false;
        bool OnMotorTurnOffBtn = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        WATER_PRESSION_IS_HIGH,
        WATER_PRESSION_IS_LOW
    };
    systemStates states = INIT;

    //actions
    struct systemActions
    {
        bool motor = false;
    };
    systemActions actions;
    
    //intern variables
    bool mode = ON_AUTO_MODE_SELECTED;
    float pression_value = 0.0;
    float prev_pression_value = 0.0;
    
 public:
    WaterPressionSystem( DisplayAdapter &displayAdapter, GenericPressionSensor &pressionSensor, GenericButton &motorTurnOnBtn, GenericButton &motorTurnOffBtn, AlertSystem &alertSystem)
        : displayAdapter(displayAdapter), pressionSensor(pressionSensor), motorTurnOnBtn(motorTurnOnBtn), motorTurnOffBtn(motorTurnOffBtn), alertSystem(alertSystem)
    {
        motor = nullptr;
    }
    ~WaterPressionSystem()
    {
        delete motor;
    }
    void updateEvents();   
    void FSM_WaterPressionSystemAutoMode();
    void FSM_WaterPressionSystemManMode();
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


void WaterPressionSystem::updateEvents(){
        pression_value = pressionSensor.getPressionSensorValue();
        if (pression_value  <= MIN_WATER_PRESSION_VALUE )
        {
            events.onWaterPression = ON_WATER_PRESSION_LOW;
            alertSystem.updateOnWaterPressionEvent(ON_WATER_PRESSION_LOW);
        }
        else if (pression_value >= Max_WATER_PRESSION_VALUE)
        {
            events.onWaterPression = ON_WATER_PRESSION_HIGH;
            alertSystem.updateOnWaterPressionEvent(ON_WATER_PRESSION_HIGH);
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


void WaterPressionSystem::FSM_WaterPressionSystemAutoMode(){
    switch (states)
    {
        case INIT : 
            if (events.onWaterPression == ON_WATER_PRESSION_HIGH)
            {
                states = WATER_PRESSION_IS_HIGH;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.onWaterPression == ON_WATER_PRESSION_LOW)
            {
                states = WATER_PRESSION_IS_LOW;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;
            }
        break;
        
        case WATER_PRESSION_IS_HIGH :
            if (events.onWaterPression == ON_WATER_PRESSION_LOW)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                states = WATER_PRESSION_IS_LOW;
            }
        break;
        
        case WATER_PRESSION_IS_LOW :
            if (events.onWaterPression == ON_WATER_PRESSION_HIGH)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = WATER_PRESSION_IS_HIGH;
            }
        break;
    }
}


void WaterPressionSystem::FSM_WaterPressionSystemManMode(){
    switch (states)
    {
        case INIT : 
            if (events.onWaterPression == ON_WATER_PRESSION_HIGH)
            {
                states = WATER_PRESSION_IS_HIGH;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.onWaterPression == ON_WATER_PRESSION_LOW)
            {
                states = WATER_PRESSION_IS_LOW;
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
        
        case WATER_PRESSION_IS_HIGH :
            if (events.onWaterPression == ON_WATER_PRESSION_LOW)
            {
                states = WATER_PRESSION_IS_LOW;
                if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                }                
            }
        break;
        
        case WATER_PRESSION_IS_LOW :
            if (events.onWaterPression == ON_WATER_PRESSION_HIGH )
            {
                states = WATER_PRESSION_IS_HIGH;
                actions.motor = TURN_OFF_MOTOR_GROUP;
            }
            else if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
            }
            else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
            }
        break;
    }
}

void WaterPressionSystem::updateActions(){
    if (pression_value != prev_pression_value )
    {
        displayAdapter.setPressionValue(pression_value);
        prev_pression_value = pression_value;
    }
    updateMotorAction(actions.motor);
}

#endif