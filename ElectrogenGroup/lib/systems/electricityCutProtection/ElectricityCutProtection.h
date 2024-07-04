#ifndef ELECTRICITYCUTPROTECTION_H
#define ELECTRICITYCUTPROTECTION_H

//components
#include "../../components/TemperatureSensor/PT100.h"
#include "../../components/Relay/MotorGroupRelay.h"
#include "../../components/Button/GenericButton.h"

//adapters
#include"../../handlers/displayAdapter/DisplayAdapter.h"

//subSystem
#include"../alertSystem/alertSystem.h"

class ElectricityCutProtection
{
 private:
    //components
    GenericButton &contactor;
    bool* motor;
    GenericButton &testButton;
    //adapter

    //subSystem

    //events
    struct systemEvents 
    {
        bool OnElectricity = false;
        bool OnTestBtn = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        STEG_ELECTRICITY_IS_WORKING,
        STEG_ELECTRICITY_IS_CUTTED
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
    
 public:
    ElectricityCutProtection(GenericButton &contactor, GenericButton &testButton)
        : contactor(contactor), testButton(testButton)
    {
        motor = nullptr;
    }
    ~ElectricityCutProtection()
    {
        delete motor;
    }
    void updateEvents();   
    void FSM_ElectricityCutProtectionAutoMode();
    void FSM_ElectricityCutProtectionManMode();
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

void ElectricityCutProtection::updateEvents(){
        //Serial.println("electricity update events");
        contactor.isPressed();
        if ( contactor.getButtonState() == ON_STEG_ELECTRICITY_CUTTED)
        {
            //Serial.println("ON_STEG_ELECTRICITY_CUTTED");
            events.OnElectricity = ON_STEG_ELECTRICITY_CUTTED;
        }
        else if (contactor.getButtonState() == ON_STEG_ELECTRICITY_WORKING)
        {
            //Serial.println("ON_STEG_ELECTRICITY_WORKING");
            events.OnElectricity = ON_STEG_ELECTRICITY_WORKING;
        }

       if( testButton.isPressed() == ON_TEST_BUTTON_CLICKED ){
           events.OnTestBtn = ON_TEST_BUTTON_CLICKED;
        }
        else if (testButton.isPressed() == ON_TEST_BUTTON_RELEASED)
        {
           events.OnTestBtn = ON_TEST_BUTTON_RELEASED; 
        }
}


void ElectricityCutProtection::FSM_ElectricityCutProtectionAutoMode(){
    switch (states)
    {
        case INIT : 
            if (events.OnElectricity == ON_STEG_ELECTRICITY_WORKING)
            {
                states = STEG_ELECTRICITY_IS_WORKING;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.OnElectricity == ON_STEG_ELECTRICITY_CUTTED)
            {
                states = STEG_ELECTRICITY_IS_CUTTED;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;
            }
        break;
        
        case STEG_ELECTRICITY_IS_WORKING :
            if (events.OnElectricity == ON_STEG_ELECTRICITY_CUTTED)
            {
                states = STEG_ELECTRICITY_IS_CUTTED;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;
            }
        break;
        
        case STEG_ELECTRICITY_IS_CUTTED :
            if (events.OnElectricity == ON_STEG_ELECTRICITY_WORKING)
            {
                states = STEG_ELECTRICITY_IS_WORKING;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
        break;
    }
}


void ElectricityCutProtection::FSM_ElectricityCutProtectionManMode(){
    switch (states)
    {
        case INIT : 
            if (events.OnElectricity == ON_STEG_ELECTRICITY_WORKING)
            {
                states = STEG_ELECTRICITY_IS_WORKING;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.OnElectricity == ON_STEG_ELECTRICITY_CUTTED)
            {
                states = STEG_ELECTRICITY_IS_CUTTED;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;
            }
        break;
        
        case STEG_ELECTRICITY_IS_WORKING :
            if (events.OnElectricity == ON_STEG_ELECTRICITY_CUTTED)
            {
                states = STEG_ELECTRICITY_IS_CUTTED;
                actions.motor = TURN_ON_MOTOR_GROUP;
                delay(3000);
            }
        break;
        
        case STEG_ELECTRICITY_IS_CUTTED :
            if (events.OnElectricity == ON_STEG_ELECTRICITY_WORKING)
            {
                states = STEG_ELECTRICITY_IS_WORKING;
                actions.motor = TURN_OFF_MOTOR_GROUP;
            }
        break;
    }
}

void ElectricityCutProtection::updateActions(){

    updateMotorAction(actions.motor);
}

#endif