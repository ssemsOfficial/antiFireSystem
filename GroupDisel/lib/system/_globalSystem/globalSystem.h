#ifndef GLOBALSYSTEM_H
#define GLOBALSYSTEM_H

//components
#include "../../components/Button/GenericButton.h"
#include "../../components/Button/SelectorButton.h"
#include "../../components/Relay/MotorGroupRelay.h"
#include "../../components/Relay/PneumaticDistributorRelay.h"
//adapters
#include"../handler/displayAdapter/DisplayAdapter.h"

//sub systems
#include "../alertSystem/alertSystem.h"
#include "../batteryLevelSelectSystem/BatteryLevelSelectSystem.h"
#include "../temperatureProtectionSystem/temperatureProtectionSystem.h"
#include "../waterPressionSystem/waterPressionSystem.h"
#include "../oilReserveSystem/oilReserveSystem.h"


class GloablSystem
{
 private:
    //components
    GenericButton &manMotorOffBtn;
    SelectorButton &buttonAutoMan;
    MotorGroupRelay &motorGroup;
    PneumaticDistributorRelay &motorBrakingSystem;

    
    //adapter
    DisplayAdapter &displayAdapter;
    
    //sub systems
    BatteryLevelSelectSystem &batteryLevelSelectSystem; 
    TemperatureProtectionSystem &temperatureProtectionSystem;
    AlertSystem &alertSystem;
    WaterPressionSystem &waterPressionSystem;
    OilReserveSystem &oilReserveSystem;


    //events
    struct SystemEvents
    {
      bool onButtonSelectMode = false;
      bool onManMotorOffBtn = false;
    };
    SystemEvents events;
        //modes
    enum SystemModes  
    {
      INIT,
      MODE_IS_AUTOMATIC,
      MODE_IS_MANUAL
    };
    SystemModes systemModes = INIT;
        //actions
    struct SystemActions  
    {
      bool affichageMode = false;
      bool motor[4] = { false, false, false, false };
      bool motorBrakingSystem = false;
    };
    SystemActions actions;


    
    
 public:
    GloablSystem(DisplayAdapter &displayAdapter, SelectorButton &buttonAutoMan, MotorGroupRelay &motorGroup, PneumaticDistributorRelay &motorBrakingSystem, BatteryLevelSelectSystem &batteryLevelSelectSystem, TemperatureProtectionSystem &temperatureProtectionSystem, WaterPressionSystem &waterPressionSystem, OilReserveSystem &oilReserveSystem, AlertSystem &alertSystem, GenericButton &manMotorOffBtn)
        : displayAdapter(displayAdapter), batteryLevelSelectSystem(batteryLevelSelectSystem), buttonAutoMan(buttonAutoMan), motorGroup(motorGroup), motorBrakingSystem(motorBrakingSystem), temperatureProtectionSystem(temperatureProtectionSystem), waterPressionSystem(waterPressionSystem), oilReserveSystem(oilReserveSystem), alertSystem(alertSystem), manMotorOffBtn(manMotorOffBtn)
    {
      oilReserveSystem.setMotor(actions.motor[RESERVE_LEVEL_CONDITION]);
      batteryLevelSelectSystem.setMotor(actions.motor[BATTERY_LEVEL_CONDITION]);
      temperatureProtectionSystem.setMotor(actions.motor[TEMPERATURE_CONDITION]);
      waterPressionSystem.setMotor(actions.motor[WATER_PRESSION_CONDITION]);
    }
    ~GloablSystem()
    {}
    void updateEvents();   
    void FSM_GloablSystem();
    void updateActions();
};


void GloablSystem::updateEvents(){
    //read inputs
    events.onButtonSelectMode = buttonAutoMan.isAutoModeSelected() ? ON_AUTO_MODE_SELECTED : ON_MAN_MODE_SELECTED;
    events.onManMotorOffBtn= manMotorOffBtn.isPressed() ? ON_TEST_BUTTON_CLICKED : ON_TEST_BUTTON_RELEASED;
    temperatureProtectionSystem.updateEvents();
    batteryLevelSelectSystem.updateEvents();
    waterPressionSystem.updateEvents();
    oilReserveSystem.updateEvents();
    alertSystem.updateEvents();
}

void GloablSystem::FSM_GloablSystem(){
  switch (systemModes)
  {
    case INIT:
      if (events.onButtonSelectMode == ON_AUTO_MODE_SELECTED)
      {
        systemModes = MODE_IS_AUTOMATIC;
        actions.affichageMode = DISPLAY_MODE_AUTO;
        break;
      }
      if (events.onButtonSelectMode == ON_MAN_MODE_SELECTED)
      {
        systemModes = MODE_IS_MANUAL;
        actions.affichageMode = DISPLAY_MODE_MAN;
        break;
      }
    break;

    case MODE_IS_AUTOMATIC:
      if (events.onButtonSelectMode == ON_MAN_MODE_SELECTED)
      {
        systemModes = MODE_IS_MANUAL;
        actions.affichageMode = DISPLAY_MODE_MAN;
        batteryLevelSelectSystem.setMode(ON_MAN_MODE_SELECTED);
        temperatureProtectionSystem.setMode(ON_MAN_MODE_SELECTED);
        waterPressionSystem.setMode(ON_MAN_MODE_SELECTED);
        oilReserveSystem.setMode(ON_MAN_MODE_SELECTED);
        break;
      }
      //FSMs
      waterPressionSystem.FSM_WaterPressionSystemAutoMode();
      temperatureProtectionSystem.FSM_TemperatureProtectionSystemAutoMode();
      batteryLevelSelectSystem.FSM_BatteryLevelSelectSystemModeAuto();
      oilReserveSystem.FSM_oilReserveSystemAutoMode();
      alertSystem.FSM_alertSystem();
    break;

    case MODE_IS_MANUAL:
      if (events.onButtonSelectMode == ON_AUTO_MODE_SELECTED)
      {
        systemModes = MODE_IS_AUTOMATIC;
        actions.affichageMode = DISPLAY_MODE_AUTO;
        waterPressionSystem.setMode(ON_AUTO_MODE_SELECTED);
        batteryLevelSelectSystem.setMode(ON_AUTO_MODE_SELECTED);
        temperatureProtectionSystem.setMode(ON_AUTO_MODE_SELECTED);
        oilReserveSystem.setMode(ON_AUTO_MODE_SELECTED);
        break;
      }
      
      if (events.onManMotorOffBtn == ON_TEST_BUTTON_CLICKED)
      {
        actions.motorBrakingSystem = TURN_OFF_MOTOR_GROUP;
      }
      else if (events.onManMotorOffBtn == ON_TEST_BUTTON_RELEASED)
      {
        actions.motorBrakingSystem = TURN_OFF_MOTOR_GROUP;
      }
      
      //FSMs
      waterPressionSystem.FSM_WaterPressionSystemManMode();
      batteryLevelSelectSystem.FSM_BatteryLevelSelectSystemModeMan();
      temperatureProtectionSystem.FSM_TemperatureProtectionSystemManMode();
      oilReserveSystem.FSM_oilReserveSystemManMode();
      alertSystem.FSM_alertSystem();

    break;    
  }
}

void GloablSystem::updateActions(){
  // write outputs
  waterPressionSystem.updateActions();
  temperatureProtectionSystem.updateActions();
  batteryLevelSelectSystem.updateActions();
  oilReserveSystem.updateActions();
  alertSystem.updateActions();
  
  
  //motor group
  if (actions.motor[BATTERY_LEVEL_CONDITION] == TURN_ON_MOTOR_GROUP && actions.motor[TEMPERATURE_CONDITION] == TURN_ON_MOTOR_GROUP && actions.motor[WATER_PRESSION_CONDITION] == TURN_ON_MOTOR_GROUP && actions.motor[RESERVE_LEVEL_CONDITION] == TURN_ON_MOTOR_GROUP && motorGroup.getRelayState() == ON_RELAY_MOTOR_GROUP_OPENED) 
  {
    Serial.println("demarrerMotorGroup");
    motorGroup.demarrerMotorGroup();
  }
  else if ( ( actions.motor[RESERVE_LEVEL_CONDITION] == TURN_OFF_MOTOR_GROUP || actions.motor[WATER_PRESSION_CONDITION] == TURN_OFF_MOTOR_GROUP || actions.motor[BATTERY_LEVEL_CONDITION] == TURN_OFF_MOTOR_GROUP ||  actions.motor[TEMPERATURE_CONDITION] == TURN_OFF_MOTOR_GROUP ) && motorGroup.getRelayState() == ON_RELAY_MOTOR_GROUP_CLOSED) 
  {
    Serial.println("arreterMotorGroup");
    motorGroup.arreterMotorGroup();
  }

  //motor group braking system
  if (actions.motorBrakingSystem == TURN_OFF_MOTOR_GROUP )
  {
    motorBrakingSystem.TurnOn();
  }
  else if (actions.motorBrakingSystem == TURN_ON_MOTOR_GROUP)
  {
    motorBrakingSystem.turnOff();
  }
  
  
  //display Man/auto
  if (actions.affichageMode == DISPLAY_MODE_AUTO)
  {
    displayAdapter.setMode("auto");
  }
  else if (actions.affichageMode == DISPLAY_MODE_MAN)
  {
    displayAdapter.setMode("man");
  }
 //delay(500);
}

#endif