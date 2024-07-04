#ifndef BATTERYLEVELSYSTEM_H
#define BATTERYLEVELSYSTEM_H

//components
#include "../../components/VoltageSensor/VoltageSensor0To25DC.h"
#include "../../components/Relay/BatteryPowerSupplyRelay.h"
#include "../../components/Relay/SelectBatPoSupRelay.h"
#include "../../components/Relay/SelectBatMotorRelay.h"
#include "../components/Button/GenericButton.h"
//adapters
#include"../handler/displayAdapter/DisplayAdapter.h"
//subSystems
#include"../alertSystem/alertSystem.h"


class BatteryLevelSelectSystem
{
 private:
    //components
    VoltageSensor0To25DC &voltageSensor1;
    VoltageSensor0To25DC &voltageSensor2;
    BatteryPowerSupplyRelay &batteryCharger;
    SelectBatPoSupRelay &batteryChargingSelector;
    SelectBatMotorRelay &batteryMotorSelector;
    GenericButton &motorTurnOnBtn;
    GenericButton &motorTurnOffBtn;
    bool* motor;
    //adapter
    DisplayAdapter &displayAdapter;
    //subSystem
    AlertSystem &alertSystem;
    //events
    struct systemEvents 
    {
        bool OnBattery1Level = false;
        bool OnBattery2Level = false;
        bool OnMotorTurnOnBtn = false;
        bool OnMotorTurnOffBtn = false;
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        BATTERY_1_AND_2_ARE_CHARGED,
        BATTERY_1_LEVEL_IS_LOW,
        BATTERY_2_LEVEL_IS_LOW,
        BATTERY_1_AND_2_ARE_LOW
    };
    systemStates states= INIT;

    //actions
    struct systemActions
    {
        bool batteryCharger = false;
        bool batteryChargingSelector = false;
        bool batteryMotorSelector = false;
        bool motor = false;
    };
    systemActions actions;

    bool mode = true;
    float battery1VoltageValue= 0.0;
    float prev_battery1VoltageValue = 0.0;
    float battery2VoltageValue= 0.0;
    float prev_battery2VoltageValue = 0.0;
 public:
    BatteryLevelSelectSystem(DisplayAdapter &displayAdapter, GenericButton &motorTurnOnBtn, GenericButton &motorTurnOffBtn, VoltageSensor0To25DC &voltageSensor1, VoltageSensor0To25DC &voltageSensor2, BatteryPowerSupplyRelay &batteryCharger, SelectBatPoSupRelay &batteryChargingSelector, SelectBatMotorRelay &batteryMotorSelector, AlertSystem &alertSystem)
        : displayAdapter(displayAdapter), motorTurnOnBtn(motorTurnOnBtn), motorTurnOffBtn(motorTurnOffBtn), voltageSensor1(voltageSensor1), voltageSensor2(voltageSensor2), batteryCharger(batteryCharger), batteryChargingSelector(batteryChargingSelector), batteryMotorSelector(batteryMotorSelector), alertSystem(alertSystem)
    {}
    ~BatteryLevelSelectSystem()
    {}
    void updateEvents();   
    void FSM_BatteryLevelSelectSystemModeAuto();
    void FSM_BatteryLevelSelectSystemModeMan();
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


void BatteryLevelSelectSystem::updateEvents(){
    battery1VoltageValue = voltageSensor1.getVoltageValue(); 
    if ( battery1VoltageValue > MIN_VOLTAGE_VALUE )
    {
        events.OnBattery1Level = ON_BATTERY_LEVEL_CHARGED;
        alertSystem.updateOnBattery1Event(ON_BATTERY_LEVEL_CHARGED);
    }
    else if (battery1VoltageValue <= MIN_VOLTAGE_VALUE)
    {
        events.OnBattery1Level = ON_BATTERY_LEVEL_LOW;
        alertSystem.updateOnBattery1Event(ON_BATTERY_LEVEL_LOW);
    }

    battery2VoltageValue = voltageSensor2.getVoltageValue(); 
    if ( battery2VoltageValue > MIN_VOLTAGE_VALUE )
    {
        events.OnBattery2Level = ON_BATTERY_LEVEL_CHARGED;
        alertSystem.updateOnBattery2Event(ON_BATTERY_LEVEL_CHARGED);
    }
    else if (battery2VoltageValue <= MIN_VOLTAGE_VALUE)
    {
        events.OnBattery2Level = ON_BATTERY_LEVEL_LOW;
        alertSystem.updateOnBattery2Event(ON_BATTERY_LEVEL_LOW);
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


void BatteryLevelSelectSystem::FSM_BatteryLevelSelectSystemModeAuto(){
    switch (states)
    {
        case INIT : 
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_CHARGED;
                break;

            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_2_LEVEL_IS_LOW;
                break;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
                states = BATTERY_1_LEVEL_IS_LOW;
                break;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_LOW;
                break;
            }
        break;
        
        case BATTERY_1_AND_2_ARE_CHARGED :
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED )
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
                states = BATTERY_1_LEVEL_IS_LOW;
            }
            else if (events.OnBattery2Level == ON_BATTERY_LEVEL_LOW && events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_2_LEVEL_IS_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_LOW;
            }
        break;
        
        case BATTERY_1_AND_2_ARE_LOW :
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED )
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
                states = BATTERY_1_LEVEL_IS_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW  )
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_2_LEVEL_IS_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_CHARGED;
            }
        break;

        case BATTERY_1_LEVEL_IS_LOW :
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW )
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_2_LEVEL_IS_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_CHARGED;
            }
        break;

        case BATTERY_2_LEVEL_IS_LOW :
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED )
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
                states = BATTERY_1_LEVEL_IS_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_LOW;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                states = BATTERY_1_AND_2_ARE_CHARGED;
            }
        break;
    }
}


void BatteryLevelSelectSystem::FSM_BatteryLevelSelectSystemModeMan(){
    switch (states)
    {
        case INIT : 
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_1_AND_2_ARE_CHARGED;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                states = BATTERY_2_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_1_LEVEL_IS_LOW;
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                states = BATTERY_1_AND_2_ARE_LOW;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                break;
            }
        break;
        
        case BATTERY_1_AND_2_ARE_CHARGED :
            Serial.println("BATTERY_1_AND_2_ARE_CHARGED");
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED )
            {
                states = BATTERY_1_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery2Level == ON_BATTERY_LEVEL_LOW && events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_2_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                states = BATTERY_1_AND_2_ARE_LOW;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                break;
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
        
        case BATTERY_1_AND_2_ARE_LOW :
            Serial.println("BATTERY_1_AND_2_ARE_LOW");
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED )
            {
                states = BATTERY_1_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW  )
            {
                states = BATTERY_2_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_1_AND_2_ARE_CHARGED;
                actions.motor = TURN_ON_MOTOR_GROUP;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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

        case BATTERY_1_LEVEL_IS_LOW :
            Serial.println("BATTERY_1_LEVEL_IS_LOW");
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW )
            {
                states = BATTERY_2_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_2_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                Serial.println(" passed to 1 and 2 low ! ");
                states = BATTERY_1_AND_2_ARE_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                Serial.println(actions.motor);
                break;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_1_AND_2_ARE_CHARGED;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
            if (events.OnMotorTurnOnBtn == ON_TEST_BUTTON_CLICKED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
            {
                actions.motor = TURN_ON_MOTOR_GROUP;
            }
            else if(events.OnMotorTurnOnBtn == ON_TEST_BUTTON_RELEASED && events.OnMotorTurnOffBtn == ON_TEST_BUTTON_RELEASED)
            {
                actions.motor = TURN_OFF_MOTOR_GROUP;
            }                
        break;

        case BATTERY_2_LEVEL_IS_LOW :
            Serial.println("BATTERY_2_LEVEL_IS_LOW");
            if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED )
            {
                states = BATTERY_1_LEVEL_IS_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_2_TO_USE_WITH_MOTOR;
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
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_LOW && events.OnBattery2Level == ON_BATTERY_LEVEL_LOW)
            {
                Serial.println(" passed to 1 and 2 low ! ");
                states = BATTERY_1_AND_2_ARE_LOW;
                actions.batteryCharger = TURN_ON_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
                actions.motor = TURN_OFF_MOTOR_GROUP;
                Serial.println(actions.motor);
                break;
            }
            else if (events.OnBattery1Level == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2Level == ON_BATTERY_LEVEL_CHARGED)
            {
                states = BATTERY_1_AND_2_ARE_CHARGED;
                actions.batteryCharger = CUT_OFF_BATTERY_POWER_SUPPLY;
                actions.batteryChargingSelector = SELECT_BATTERY_1_TO_CHARGE;
                actions.batteryMotorSelector = SELECT_BATTERY_1_TO_USE_WITH_MOTOR;
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
}


void BatteryLevelSelectSystem::updateActions(){

    if(battery1VoltageValue != prev_battery1VoltageValue){
        prev_battery1VoltageValue = battery1VoltageValue;
        displayAdapter.setBattery1Level(battery1VoltageValue);
    }
    
    if(battery2VoltageValue != prev_battery2VoltageValue){
        prev_battery2VoltageValue = battery2VoltageValue;
        displayAdapter.setBattery2Level(battery2VoltageValue);
    }
    

    if (actions.batteryCharger == TURN_ON_BATTERY_POWER_SUPPLY )
    {
        //displayAdapter.setBatteryLevel((float)voltageSensor.getVoltageValue());
        batteryCharger.turnOnPowerSupply();
    }
    else if (actions.batteryCharger == CUT_OFF_BATTERY_POWER_SUPPLY)
    {
        //displayAdapter.setBatteryLevel((float)voltageSensor.getVoltageValue());
        batteryCharger.cutOffPowerSupply();
    }

    if (actions.batteryChargingSelector == SELECT_BATTERY_1_TO_CHARGE )
    {
        //displayAdapter.setBatteryLevel((float)voltageSensor.getVoltageValue());
        batteryChargingSelector.selectToChargeBat1();
    }
    else if (actions.batteryChargingSelector == SELECT_BATTERY_2_TO_CHARGE)
    {
        //displayAdapter.setBatteryLevel((float)voltageSensor.getVoltageValue());
        batteryChargingSelector.selectToChargeBat2();
    }

    if (actions.batteryMotorSelector == SELECT_BATTERY_1_TO_USE_WITH_MOTOR )
    {
        //displayAdapter.setBatteryLevel((float)voltageSensor.getVoltageValue());
        batteryMotorSelector.selectToUseBat1();
    }
    else if (actions.batteryMotorSelector == SELECT_BATTERY_2_TO_USE_WITH_MOTOR)
    {
        //displayAdapter.setBatteryLevel((float)voltageSensor.getVoltageValue());
        batteryMotorSelector.selectToUseBat2();
    }
    updateMotorAction(actions.motor);
}

#endif