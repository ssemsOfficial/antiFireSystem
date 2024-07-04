#ifndef BATTERYLEVELSYSTEM_H
#define BATTERYLEVELSYSTEM_H

//components
#include "../../components/VoltageSensor/VoltageSensor0To25DC.h"
#include "../../components/Relay/BatteryPowerSupplyRelay.h"
#include "../../components/Relay/MotorGroupRelay.h"
//adapters
#include"../../handlers/displayAdapter/DisplayAdapter.h"
//subSystems
#include"../alertSystem/alertSystem.h"


class BatteryLevelSystem
{
 private:
    //components
    VoltageSensor0To25DC &voltageSensor;
    BatteryPowerSupplyRelay &battery;
    GenericButton &testButton;
    bool* motor;
    //adapter
    DisplayAdapter &displayAdapter;
    //subSystem
    AlertSystem &alertSystem;
    //events
    struct systemEvents 
    {
        bool OnBatteryLevel = false;
        bool OnTestBtn = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        BATTERY_LEVEL_IS_CHARGED,
        BATTERY_LEVEL_IS_LOW
    };
    systemStates states= INIT;

    //actions
    struct systemActions
    {
        bool battery = false;
        bool motor = false;
    };
    systemActions actions;

    bool mode = ON_AUTO_MODE_SELECTED;
    
    float batteryVoltageValue = 0.0;
    float prev_batteryVoltageValue = 0.0;

 public:
    BatteryLevelSystem(AlertSystem &alertSystem, DisplayAdapter &displayAdapter, VoltageSensor0To25DC &voltageSensor, BatteryPowerSupplyRelay &battery, GenericButton &testButton)
        : alertSystem(alertSystem), displayAdapter(displayAdapter), voltageSensor(voltageSensor), battery(battery), testButton(testButton)
    {
        motor = nullptr;
    }
    ~BatteryLevelSystem()
    {
        delete motor;
    }
    void updateEvents();   
    void FSM_BatteryLevelSystemAutoMode();
    void FSM_BatteryLevelSystemManMode();
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


void BatteryLevelSystem::updateEvents(){
    batteryVoltageValue = voltageSensor.getVoltageValue(); 
    if (  batteryVoltageValue > MIN_VOLTAGE_VALUE )
    {
        events.OnBatteryLevel = ON_BATTERY_LEVEL_CHARGED;
        alertSystem.updateOnBatteryEvent(ON_BATTERY_LEVEL_CHARGED);
    }
    else if (batteryVoltageValue <= MIN_VOLTAGE_VALUE)
    {
        events.OnBatteryLevel = ON_BATTERY_LEVEL_LOW;
        alertSystem.updateOnBatteryEvent(ON_BATTERY_LEVEL_LOW);
    }

    if (testButton.isPressed() == ON_TEST_BUTTON_CLICKED)
    {
        events.OnTestBtn = ON_TEST_BUTTON_CLICKED;
    }
    else if (testButton.isPressed() == ON_TEST_BUTTON_RELEASED)
    {
        events.OnTestBtn = ON_TEST_BUTTON_RELEASED;
    }
}


void BatteryLevelSystem::FSM_BatteryLevelSystemAutoMode(){
    switch (states)
    {
        case INIT : 
            if (events.OnBatteryLevel == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_LEVEL_IS_CHARGED;
                actions.battery = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.motor = TURN_ON_MOTOR_GROUP;
                break;

            }
            else if (events.OnBatteryLevel== ON_BATTERY_LEVEL_LOW)
            {
                states = BATTERY_LEVEL_IS_LOW;
                actions.battery = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
        break;
        
        case BATTERY_LEVEL_IS_CHARGED :
            if (events.OnBatteryLevel == ON_BATTERY_LEVEL_LOW)
            {
                actions.battery = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = BATTERY_LEVEL_IS_LOW;
            }
        break;
        
        case BATTERY_LEVEL_IS_LOW :
            if (events.OnBatteryLevel == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.battery = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.motor = TURN_ON_MOTOR_GROUP;
                states = BATTERY_LEVEL_IS_CHARGED;
            }
        break;
    }
}

void BatteryLevelSystem::FSM_BatteryLevelSystemManMode(){
    switch (states)
    {
        case INIT : 
            if (events.OnBatteryLevel== ON_BATTERY_LEVEL_LOW)
            {
                states = BATTERY_LEVEL_IS_LOW;
                actions.battery = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                break;
            }
            else if (events.OnBatteryLevel == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_LEVEL_IS_CHARGED;
                actions.battery = CUT_OFF_BATTERY_POWER_SUPPLY;
                if (events.OnTestBtn == ON_TEST_BUTTON_CLICKED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if (events.OnTestBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;    
                }
                break;
            }
            
        break;
        
        case BATTERY_LEVEL_IS_CHARGED :
            if (events.OnBatteryLevel == ON_BATTERY_LEVEL_LOW)
            {
                actions.battery = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                states = BATTERY_LEVEL_IS_LOW;
            }
            else{
                if (events.OnTestBtn == ON_TEST_BUTTON_CLICKED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if (events.OnTestBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
            }
        break;
        
        case BATTERY_LEVEL_IS_LOW :
            if (events.OnBatteryLevel == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_LEVEL_IS_CHARGED;
                actions.battery = CUT_OFF_BATTERY_POWER_SUPPLY;
                if (events.OnTestBtn == ON_TEST_BUTTON_CLICKED)
                {
                    actions.motor = TURN_ON_MOTOR_GROUP;
                }
                else if (events.OnTestBtn == ON_TEST_BUTTON_RELEASED)
                {
                    actions.motor = TURN_OFF_MOTOR_GROUP;
                }
            }
        break;
    }
}

void BatteryLevelSystem::updateActions(){
    if(batteryVoltageValue != prev_batteryVoltageValue){
        prev_batteryVoltageValue = batteryVoltageValue;
        displayAdapter.setBatteryLevel(batteryVoltageValue);
    }
    if (actions.battery == CUT_OFF_BATTERY_POWER_SUPPLY)
    {
        battery.cutOffPowerSupply();
    }
    else if (actions.battery == TURN_ON_BATTERY_POWER_SUPPLY)
    {
        battery.turnOnPowerSupply();
    }
    updateMotorAction(actions.motor);   
}

#endif