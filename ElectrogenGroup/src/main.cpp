//Framework
#include <Arduino.h>

//BSP
#include<../lib/BSP.h>

//extern
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <EthernetENC.h>

//components
#include<../lib/components/Relay/PompeRelay.h>
#include<../lib/components/Flottor/GenericFlottor.h>
#include<../lib/components/TemperatureSensor/PT100.h>
#include<../lib/components/Relay/MotorGroupRelay.h>
#include<../lib/components/VoltageSensor/VoltageSensor0To25DC.h>
#include<../lib/components/Relay/BatteryPowerSupplyRelay.h>
#include<../lib/components/Button/SelectorButton.h>
#include<../lib/components/Buzzer/GenericBuzzer.h>
#include<../lib/components/Led/GenericLed.h>

//adapters
#include<../lib/handlers/displayAdapter/DisplayAdapter.h>

//systems
 //global
#include<../lib/systems/_globalSystem/globalSystem.h> 
 //sub systems
#include<../lib/systems/oilReserveSystem/oilReserveSystem.h>
#include<../lib/systems/temperatureProtectionSystem/temperatureProtectionSystem.h>
#include<../lib/systems/batteryLevelSystem/BatteryLevelSystem.h>
#include<../lib/systems/electricityCutProtection/ElectricityCutProtection.h>
#include<../lib/systems/alertSystem/alertSystem.h>


//declarations
 //inputs
SelectorButton buttonAutoMan(buttonAutoManPin); 
GenericButton stegContactor(stegContactorPin);
GenericButton testMotorGroupButton(testMotorGroupButtonPin); 
GenericButton resetAlertButton(resetAlertButtonPin);
GenericFlottor flottor(flottorPin);
PT100  pt100(pt100Pin);
VoltageSensor0To25DC voltageSensor(voltageSensorPin);
 //outputs
MotorGroupRelay motorGroup(motorGroupRelayPin); 
PompeRelay pump(pumpRelayPin);
BatteryPowerSupplyRelay battery(batteryPowerSupplyRelayPin);
LiquidCrystal_I2C lcd(0x27,20,4);
GenericBuzzer buzzer(buzzerAlertPin);
GenericLed led(ledAlertPin);
 //intern variables

  //adapters
DisplayAdapter displayAdapter(lcd);
 //systems
  //sub
AlertSystem alertSystem(displayAdapter, buzzer, led, resetAlertButton);  
ElectricityCutProtection electricityCutProtection(stegContactor, testMotorGroupButton);
OilReserveSystem oilReserveSystem(alertSystem, displayAdapter, flottor, testMotorGroupButton, pump);
TemperatureProtectionSystem temperatureProtectionSystem(alertSystem, displayAdapter, pt100, testMotorGroupButton);
BatteryLevelSystem batteryLevelSystem(alertSystem, displayAdapter, voltageSensor, battery, testMotorGroupButton);
  //global
GloablSystem globalSystem(alertSystem,displayAdapter,buttonAutoMan,motorGroup,oilReserveSystem,batteryLevelSystem,temperatureProtectionSystem,electricityCutProtection);

void setup() {
  //init material
  Serial.begin(baud_rate);
  pump.init();
  flottor.init();
  motorGroup.init();
  pt100.init();
  battery.init();
  voltageSensor.init();
  buttonAutoMan.init();
  led.init();
  buzzer.init();
  stegContactor.init();
  testMotorGroupButton.init();
  resetAlertButton.init();
  lcd.init();
  lcd.clear();

}

void loop() {
  globalSystem.updateEvents();
  globalSystem.FSM_GloablSystem();
  globalSystem.updateActions();
}

