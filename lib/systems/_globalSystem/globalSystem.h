#ifndef GLOBALSYSTEM_H
#define GLOBALSYSTEM_H

//components
#include "../../components/Button/SelectorButton.h"
#include "../../components/Relay/MotorGroupRelay.h"
//adapters
#include"../../handlers/displayAdapter/DisplayAdapter.h"

//sub systems
#include "../batteryLevelSystem/BatteryLevelSystem.h"
#include "../oilReserveSystem/oilReserveSystem.h"
#include "../temperatureProtectionSystem/temperatureProtectionSystem.h"
#include "../electricityCutProtection/ElectricityCutProtection.h"
#include "../alertSystem/alertSystem.h"

class GloablSystem
{
 private:
    //components
    SelectorButton &buttonAutoMan;
    MotorGroupRelay motorGroup;
    //adapter
    DisplayAdapter &displayAdapter;
    //sub systems
    OilReserveSystem &oilReserveSystem; 
    BatteryLevelSystem &batteryLevelSystem; 
    TemperatureProtectionSystem &temperatureProtectionSystem;
    ElectricityCutProtection &electricityCutProtection;
    AlertSystem &alertSystem;
    //events
    struct SystemEvents
    {
    bool onButtonSelectMode = false;
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
      bool motor[4] = {false, false, false, false};
    };
    SystemActions actions;


    
    
 public:
    GloablSystem(AlertSystem &alertSystem, DisplayAdapter &displayAdapter,SelectorButton &buttonAutoMan, MotorGroupRelay &motorGroup, OilReserveSystem &oilReserveSystem, BatteryLevelSystem &batteryLevelSystem, TemperatureProtectionSystem &temperatureProtectionSystem, ElectricityCutProtection &electricityCutProtection)
        : alertSystem(alertSystem), displayAdapter(displayAdapter), oilReserveSystem(oilReserveSystem), batteryLevelSystem(batteryLevelSystem), temperatureProtectionSystem(temperatureProtectionSystem), buttonAutoMan(buttonAutoMan), motorGroup(motorGroup), electricityCutProtection(electricityCutProtection)
    {
      oilReserveSystem.setMotor(actions.motor[RESERVE_LEVEL_CONDITION]);
      batteryLevelSystem.setMotor(actions.motor[BATTERY_LEVEL_CONDITION]);
      temperatureProtectionSystem.setMotor(actions.motor[TEMPERATURE_CONDITION]);
      electricityCutProtection.setMotor(actions.motor[STEG_ELECTRICITY_CONDITION]);
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
    electricityCutProtection.updateEvents();
    oilReserveSystem.updateEvents();
    temperatureProtectionSystem.updateEvents();
    batteryLevelSystem.updateEvents();
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
        oilReserveSystem.setMode(ON_MAN_MODE_SELECTED);
        batteryLevelSystem.setMode(ON_MAN_MODE_SELECTED);
        temperatureProtectionSystem.setMode(ON_MAN_MODE_SELECTED);
        electricityCutProtection.setMode(ON_MAN_MODE_SELECTED);
        break;
      }
      //FSMs
      electricityCutProtection.FSM_ElectricityCutProtectionAutoMode();
      oilReserveSystem.FSM_oilReserveSystemAutoMode();
      temperatureProtectionSystem.FSM_TemperatureProtectionSystemAutoMode();
      batteryLevelSystem.FSM_BatteryLevelSystemAutoMode();
      alertSystem.FSM_alertSystem();
    break;

    case MODE_IS_MANUAL:
      if (events.onButtonSelectMode == ON_AUTO_MODE_SELECTED)
      {
        systemModes = MODE_IS_AUTOMATIC;
        actions.affichageMode = DISPLAY_MODE_AUTO;
        electricityCutProtection.setMode(ON_AUTO_MODE_SELECTED);
        oilReserveSystem.setMode(ON_AUTO_MODE_SELECTED);
        batteryLevelSystem.setMode(ON_AUTO_MODE_SELECTED);
        temperatureProtectionSystem.setMode(ON_AUTO_MODE_SELECTED);
        break;
      }
      //FSMs
      electricityCutProtection.FSM_ElectricityCutProtectionManMode();
      oilReserveSystem.FSM_oilReserveSystemManMode();
      batteryLevelSystem.FSM_BatteryLevelSystemManMode();
      temperatureProtectionSystem.FSM_TemperatureProtectionSystemManMode();
      alertSystem.FSM_alertSystem();
    break;    
  }
}

void GloablSystem::updateActions(){
  // write outputs
  electricityCutProtection.updateActions();
  oilReserveSystem.updateActions();  
  temperatureProtectionSystem.updateActions();
  batteryLevelSystem.updateActions();
  alertSystem.updateActions();
for (size_t i = 0; i < 4; i++)
  {
    Serial.print(actions.motor[i]);
  }
  
  Serial.println("");
  if (actions.motor[STEG_ELECTRICITY_CONDITION] == TURN_ON_MOTOR_GROUP && actions.motor[TEMPERATURE_CONDITION] == TURN_ON_MOTOR_GROUP && actions.motor[BATTERY_LEVEL_CONDITION] == TURN_ON_MOTOR_GROUP && actions.motor[RESERVE_LEVEL_CONDITION] == TURN_ON_MOTOR_GROUP && motorGroup.getRelayState() == ON_RELAY_MOTOR_GROUP_OPENED) 
  {
    motorGroup.demarrerMotorGroup();
  }
  else if ( (actions.motor[STEG_ELECTRICITY_CONDITION] == TURN_OFF_MOTOR_GROUP || actions.motor[TEMPERATURE_CONDITION] == TURN_OFF_MOTOR_GROUP || actions.motor[BATTERY_LEVEL_CONDITION] == TURN_OFF_MOTOR_GROUP || actions.motor[RESERVE_LEVEL_CONDITION] == TURN_OFF_MOTOR_GROUP) && motorGroup.getRelayState() == ON_RELAY_MOTOR_GROUP_CLOSED) 
  {
    motorGroup.arreterMotorGroup();
  }

  if (actions.affichageMode == DISPLAY_MODE_AUTO)
  {
    displayAdapter.setMode("auto");
  }
  else if (actions.affichageMode == DISPLAY_MODE_MAN)
  {
    displayAdapter.setMode("man");
  }
}

#endif