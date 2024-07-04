#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H



//components
#include "../../components/Buzzer/GenericBuzzer.h"
#include "../../components/Led/GenericLed.h"
#include "../../components/Button/GenericButton.h"

//adapters
#include "../handler/displayAdapter/DisplayAdapter.h"


class AlertSystem
{
 private:
    //components
    GenericBuzzer &buzzer;
    GenericLed &led;
    GenericButton &resetAlertButton;
    //adapters
    DisplayAdapter &display;    
    //events
    struct systemEvents 
    {
        bool OnReserve = false;
        bool OnBattery1 = false;
        bool OnBattery2 = false;
        bool OnTemperature = false;
        bool OnWaterPression = false;
        bool OnResetAlertButton = false;
        
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        INIT,
        SYSTEM_IS_STABLE,
        BATTERY_1_AND_2_ARE_LOW,
        RESERVE_IS_EMPTY,
        TEMPERATURE_IS_IN_DANGER_VALUE,
        WATER_PRESSION_IS_LOW
    };
    systemStates states = SYSTEM_IS_STABLE;
    
    //actions
    struct systemActions
    {
        bool buzzer = false;
        bool led = false;
        int display = false;
    };
    systemActions actions;
    

 public:
    AlertSystem(DisplayAdapter &display, GenericBuzzer &buzzer, GenericLed &led, GenericButton &resetAlertButton)
        : display(display), buzzer(buzzer), led(led), resetAlertButton(resetAlertButton)
    {}
    ~AlertSystem()
    {}

    void updateEvents();
    void updateOnBattery1Event(bool e);
    void updateOnBattery2Event(bool e);
    void updateOnTemperatureEvent(bool e);
    void updateOnReserveEvent(bool e);
    void updateOnWaterPressionEvent(bool e);
    void FSM_alertSystem();
    void updateActions();

    void setAlertDisplay(String alertType){
        display.setAlert(alertType);
    }
};


void AlertSystem::updateOnBattery1Event(bool e){
    events.OnBattery1 = e;
}
void AlertSystem::updateOnBattery2Event(bool e){
    events.OnBattery2 = e;
}
void AlertSystem::updateOnTemperatureEvent(bool e){
    events.OnTemperature = e;
}

void AlertSystem::updateOnReserveEvent(bool e){
    events.OnReserve = e;
}

void AlertSystem::updateOnWaterPressionEvent(bool e){
    events.OnWaterPression = e;
}

void AlertSystem::updateEvents(){
    if (resetAlertButton.isPressed() == ON_RESET_ALERT_BUTTON_CLICKED)
    {
        events.OnResetAlertButton = ON_RESET_ALERT_BUTTON_CLICKED;
    }
    else if(resetAlertButton.isPressed() == ON_RESET_ALERT_BUTTON_RELEASED)
    {
        events.OnResetAlertButton = ON_RESET_ALERT_BUTTON_RELEASED;
    }
}

void AlertSystem::FSM_alertSystem(){
    switch (states)
    {
        case INIT : 
            if(events.OnBattery1 == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2 == ON_BATTERY_LEVEL_CHARGED && events.OnReserve == ON_RESERVE_FILLED && events.OnTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE && events.OnWaterPression == ON_WATER_PRESSION_HIGH){
                states = SYSTEM_IS_STABLE;
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            
            else if(events.OnBattery1 == ON_BATTERY_LEVEL_LOW && events.OnBattery2 == ON_BATTERY_LEVEL_LOW){
                states = BATTERY_1_AND_2_ARE_LOW;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            
            else if(events.OnReserve == ON_RESERVE_EMPTIED){
                states = RESERVE_IS_EMPTY;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            else if(events.OnTemperature == ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE){
                states = TEMPERATURE_IS_IN_DANGER_VALUE;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            else if(events.OnWaterPression == ON_WATER_PRESSION_LOW ){
                states = WATER_PRESSION_IS_LOW;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            
        break;

        case SYSTEM_IS_STABLE :
            if (events.OnResetAlertButton == ON_RESET_ALERT_BUTTON_CLICKED)
            {
                actions.display = DISPLAY_NO_ALERT;
            }

            if(events.OnBattery1 == ON_BATTERY_LEVEL_LOW && events.OnBattery2 == ON_BATTERY_LEVEL_LOW){
                states = BATTERY_1_AND_2_ARE_LOW;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            else if(events.OnReserve == ON_RESERVE_EMPTIED){
                states = RESERVE_IS_EMPTY;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            else if(events.OnTemperature == ON_SYSTEM_TEMPERATURE_IN_DANGER_VALUE){
                states = TEMPERATURE_IS_IN_DANGER_VALUE;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
            else if(events.OnWaterPression == ON_WATER_PRESSION_LOW ){
                states = WATER_PRESSION_IS_LOW;
                //display.setDisplayAnAlert(true);
                actions.buzzer = TURN_ON_BUZZER;
                actions.led = TURN_ON_LED;
                break;
            }
        break;

        case BATTERY_1_AND_2_ARE_LOW :
            if (events.OnResetAlertButton != ON_RESET_ALERT_BUTTON_CLICKED)
            {
                actions.display = DISPLAY_ALERT_BAT1_AND_BAT2_LOW;
            }
            
            if (events.OnBattery1 == ON_BATTERY_LEVEL_CHARGED && events.OnBattery2 == ON_BATTERY_LEVEL_CHARGED)
            {
                states = SYSTEM_IS_STABLE;
                //display.setDisplayAnAlert(false);
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_OFF_LED;
            }
        break;
        
        case RESERVE_IS_EMPTY :
            if (events.OnResetAlertButton != ON_RESET_ALERT_BUTTON_CLICKED)
            {
                actions.display = DISPLAY_ALERT_RESERVE_EMPTY;
            }
            
            
            if (events.OnReserve == ON_RESERVE_FILLED)
            {
                states = SYSTEM_IS_STABLE;
                //display.setDisplayAnAlert(false);
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_OFF_LED;
            }
        break;

        case TEMPERATURE_IS_IN_DANGER_VALUE :
            if (events.OnResetAlertButton != ON_RESET_ALERT_BUTTON_CLICKED)
            {
                actions.display = DISPLAY_ALERT_TEMPERATURE;
            }
            
            
            if (events.OnTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE)
            {
                states = SYSTEM_IS_STABLE;
                //display.setDisplayAnAlert(false);
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_OFF_LED;
            }
        break;

        case WATER_PRESSION_IS_LOW :
            if (events.OnResetAlertButton != ON_RESET_ALERT_BUTTON_CLICKED)
            {
                actions.display = DISPLAY_ALERT_WATER_PRESSION;
            }

            if (events.OnWaterPression == ON_WATER_PRESSION_HIGH)
            {
                states = SYSTEM_IS_STABLE;
                //display.setDisplayAnAlert(false);
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_OFF_LED;
            }
        break;
    }
}

void AlertSystem::updateActions(){
    if (actions.buzzer == TURN_ON_BUZZER)
    {
        buzzer.activateBuzzer();
    }
    else if (actions.buzzer == TURN_OFF_BUZZER)
    {
        buzzer.desactivateBuzzer();
    }
    if (actions.led == TURN_ON_LED)
    {
        led.turnOnLed();
    }
    else if (actions.led == TURN_OFF_LED)
    {
        led.turnOffLed();
    }
 
    if(actions.display == DISPLAY_NO_ALERT){
        display.setNoAlert();
    }
    else if (actions.display == DISPLAY_ALERT_BAT1_AND_BAT2_LOW)
    {
        display.setAlert("battery");
    }
    else if(actions.display == DISPLAY_ALERT_RESERVE_EMPTY)
    {
        display.setAlert("reserve");
    }
    else if (actions.display == DISPLAY_ALERT_TEMPERATURE)
    {
        display.setAlert("temperature");
    }
    else if (actions.display == DISPLAY_ALERT_WATER_PRESSION)
    {
        display.setAlert("water pression");
    }

}

#endif