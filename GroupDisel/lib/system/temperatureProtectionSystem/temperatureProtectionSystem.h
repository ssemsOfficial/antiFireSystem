#ifndef TEMPERATUREPROCTECTIONSYSTEM_H
#define TEMPERATUREPROCTECTIONSYSTEM_H

//components
#include "../../components/TemperatureSensor/PT100.h"
#include "../../components/Relay/MotorGroupRelay.h"
#include "../../components/Button/GenericButton.h"
//adapters
#include"../handler/displayAdapter/DisplayAdapter.h"
//subSystem
#include "../alertSystem/alertSystem.h"

class TemperatureProtectionSystem
{
 private:
    //components
    PT100 &tempSensor;
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
        bool OnSystemTemperature = false;
        bool OnMotorTurnOnBtn = false;
        bool OnMotorTurnOffBtn = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE,
        SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE
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
    float temp_value = 0.0;
    float prev_temp_value = 0.0;
    
 public:
    TemperatureProtectionSystem( DisplayAdapter &displayAdapter, PT100 &tempSensor, GenericButton &motorTurnOnBtn, GenericButton &motorTurnOffBtn, AlertSystem &alertSystem)
        : displayAdapter(displayAdapter), tempSensor(tempSensor), motorTurnOnBtn(motorTurnOnBtn), motorTurnOffBtn(motorTurnOffBtn), alertSystem(alertSystem)
    {
        motor = nullptr;
    }
    ~TemperatureProtectionSystem()
    {
        delete motor;
    }
    void updateEvents();   
    void FSM_TemperatureProtectionSystemAutoMode();
    void FSM_TemperatureProtectionSystemManMode();
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


void TemperatureProtectionSystem::updateEvents(){
        temp_value = tempSensor.getTemperatureValue();
        //Serial.println(temp_value);
        if (temp_value  >= TEMPERATURE_DANGER_VALUE )
        {
            events.OnSystemTemperature = ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE;
            alertSystem.updateOnTemperatureEvent(ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE);
        }
        else if (temp_value < TEMPERATURE_DANGER_VALUE)
        {
            events.OnSystemTemperature = ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE;
            alertSystem.updateOnTemperatureEvent(ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE);
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




void TemperatureProtectionSystem::FSM_TemperatureProtectionSystemAutoMode(){
    switch (states)
    {
        case INIT : 
            if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE)
            {
                states = SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;
            }
            else if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE)
            {
                states = SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
        break;
        
        case SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE :
            if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                states = SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE;
            }
        break;
        
        case SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE :
            if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE;
            }
        break;
    }
}


void TemperatureProtectionSystem::FSM_TemperatureProtectionSystemManMode(){
    switch (states)
    {
        case INIT : 
            if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE)
            {
                states = SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE)
            {
                states = SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE;
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
        
        case SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE :
            if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE)
            {
                states = SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE;
                if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_CLICKED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                }                
            }
        break;
        
        case SYSTEM_TEMPERATURE_IS_IN_SECURE_VALUE :
            if (events.OnSystemTemperature == ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE )
            {
                states = SYSTEM_TEMPERATURE_IS_IN_DANGER_VALUE;
                actions.motor = TURN_OFF_MOTOR_GROUP;
            }
            else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED){
                actions.motor = TURN_ON_MOTOR_GROUP;
            }
            else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_CLICKED){
                actions.motor = TURN_OFF_MOTOR_GROUP;
            }
        break;
    }
}


void TemperatureProtectionSystem::updateActions(){
    if (temp_value != prev_temp_value )
    {
        displayAdapter.setTemperatureValue(temp_value);
        prev_temp_value = temp_value;
    }
    updateMotorAction(actions.motor);
}

#endif