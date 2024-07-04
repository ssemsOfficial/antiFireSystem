#ifndef ALERTSYSTEM_H
#define ALERTSYSTEM_H



//components
#include "../../components/Buzzer/GenericBuzzer.h"
#include "../../components/Led/GenericLed.h"
#include "../../components/Button/GenericButton.h"
//adapters
#include "../../handlers/displayAdapter/DisplayAdapter.h"


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
        bool OnBattery = false;
        bool OnTemperature = false;
        bool OnResetAlertButton = false;
        
    };
    systemEvents events;
    
    //system states
    enum systemStates
    {
        SYSTEM_IS_STABLE,
        BATTERY_IS_LOW,
        RESERVE_IS_EMPTY,
        TEMPERATURE_IS_IN_DANGER_VALUE
    };
    systemStates states = SYSTEM_IS_STABLE;
    
    //actions
    struct systemActions
    {
        bool buzzer = false;
        bool led = false;
    };
    systemActions actions;
    

 public:
    AlertSystem(DisplayAdapter &display, GenericBuzzer &buzzer, GenericLed &led, GenericButton &resetAlertButton)
        : display(display), buzzer(buzzer), led(led), resetAlertButton(resetAlertButton)
    {}
    ~AlertSystem()
    {}

    void updateEvents();
    void updateOnBatteryEvent(bool e);
    void updateOnTemperatureEvent(bool e);
    void updateOnReserveEvent(bool e);
    void FSM_alertSystem();
    void updateActions();

    void setAlertDisplay(String alertType){
        display.setAlert(alertType);
    }
};


void AlertSystem::updateOnBatteryEvent(bool e){
    events.OnBattery = e;
}
void AlertSystem::updateOnTemperatureEvent(bool e){
    events.OnTemperature = e;
}
void AlertSystem::updateOnReserveEvent(bool e){
    events.OnReserve = e;
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
        case SYSTEM_IS_STABLE :
            if (events.OnResetAlertButton == ON_RESET_ALERT_BUTTON_CLICKED)
            {
                display.setNoAlert();
            }
            
            if(events.OnBattery == ON_BATTERY_LEVEL_LOW){
                states = BATTERY_IS_LOW;
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
        break;

        case BATTERY_IS_LOW :
            display.setAlert("battery");
            if (events.OnBattery == ON_BATTERY_LEVEL_CHARGED)
            {
                states = SYSTEM_IS_STABLE;
                //display.setDisplayAnAlert(false);
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_OFF_LED;
            }
        break;
        
        case RESERVE_IS_EMPTY :
            display.setAlert("reserve");
            if (events.OnReserve == ON_RESERVE_FILLED)
            {
                states = SYSTEM_IS_STABLE;
                //display.setDisplayAnAlert(false);
                actions.buzzer = TURN_OFF_BUZZER;
                actions.led = TURN_OFF_LED;
            }
        break;

        case TEMPERATURE_IS_IN_DANGER_VALUE :
            display.setAlert("temperature");
            if (events.OnTemperature == ON_SYSTEM_TEMPERATURE_IN_SECURE_VALUE)
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
}

#endif