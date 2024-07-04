//Framework
#include <Arduino.h>

//BSP
#include<../lib/BSP.h>

//extern
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<SPI.h>
//#include<EthernetENC.h>

//components
#include<../lib/components/Relay/MotorGroupRelay.h>
#include<../lib/components/Button/SelectorButton.h>
#include<../lib/components/VoltageSensor/VoltageSensor0To25DC.h>
#include<../lib/components/Relay/BatteryPowerSupplyRelay.h>
#include<../lib/components/Relay/SelectBatMotorRelay.h>
#include<../lib/components/Relay/SelectBatPoSupRelay.h>
#include<../lib/components/TemperatureSensor/PT100.h>
#include<../lib/components/WaterPressionSensor/GenericPressionSensor.h>
#include<../lib/components/Flottor/GenericFlottor.h>
#include<../lib/components/Led/GenericLed.h>
#include<../lib/components/Buzzer/GenericBuzzer.h>
#include<../lib/components/Relay/PneumaticDistributorRelay.h>


//adapters
#include<../handler/displayAdapter/DisplayAdapter.h>

//systems
 //global
#include<../lib/system/_globalSystem/globalSystem.h> 
 //sub systems
#include<../lib/system/batteryLevelSelectSystem/BatteryLevelSelectSystem.h>
#include<../lib/system/temperatureProtectionSystem/temperatureProtectionSystem.h>
#include<../lib/system/waterPressionSystem/waterPressionSystem.h>
#include<../lib/system/oilReserveSystem/oilReserveSystem.h>
#include<../lib/system/alertSystem/alertSystem.h>


//declarations
 //inputs
VoltageSensor0To25DC voltageSensor1(voltageSensor1Pin);
VoltageSensor0To25DC voltageSensor2(voltageSensor2Pin);
PT100 pt100(pt100Pin); 
GenericButton manMotorOnBtn(manMotorOnBtnPin); 
GenericButton manMotorOffBtn(manMotorOffBtnPin); 
GenericPressionSensor waterPressionSensor(waterPressionSensorPin); 
GenericFlottor flottor(flottorPin); 
GenericLed led(ledAlertPin);
GenericBuzzer buzzer(buzzerAlertPin);
GenericButton resetAlertButton(resetAlertButtonPin);
 //outputs
BatteryPowerSupplyRelay batteryPowerSupply(PowerSupplyRelayPin);
SelectBatMotorRelay batteryMotorSelector(batMotorRelayPin);
SelectBatPoSupRelay batteryPoSuppSelector(batPowerSupplyRelayPin);
MotorGroupRelay motorGroup(motorGroupRelayPin); 
PneumaticDistributorRelay motorBrakingSystem(motorBrakingSystemPin);
SelectorButton buttonAutoMan(buttonAutoManPin);
LiquidCrystal_I2C lcd(0x27, 20, 4);
 //intern variables

  //adapters
DisplayAdapter displayAdapter(lcd);

 //systems
  //sub
AlertSystem alertSystem(displayAdapter, buzzer, led, resetAlertButton);  
BatteryLevelSelectSystem batteryLevelAndSelectSystem(displayAdapter, manMotorOnBtn, voltageSensor1, voltageSensor2, batteryPowerSupply, batteryPoSuppSelector, batteryMotorSelector, alertSystem);  
TemperatureProtectionSystem temperatureProtectionSystem(displayAdapter, pt100, manMotorOnBtn, manMotorOffBtn, alertSystem);
WaterPressionSystem waterPressionSystem(displayAdapter, waterPressionSensor, manMotorOnBtn, alertSystem);
OilReserveSystem oilReserveSystem(displayAdapter, flottor, manMotorOnBtn, alertSystem);
  //global
GloablSystem globalSystem( displayAdapter, buttonAutoMan, motorGroup, motorBrakingSystem, batteryLevelAndSelectSystem, temperatureProtectionSystem, waterPressionSystem, oilReserveSystem, alertSystem, manMotorOffBtn);


void setup() {
  //init material
  Serial.begin(baud_rate);
  voltageSensor1.init();
  voltageSensor2.init();
  batteryPowerSupply.init();
  batteryPoSuppSelector.init();
  batteryMotorSelector.init();
  motorGroup.init();
  motorBrakingSystem.init();
  buttonAutoMan.init();
  manMotorOnBtn.init();
  manMotorOffBtn.init();
  pt100.init();
  waterPressionSensor.init();
  flottor.init();
  buzzer.init();
  led.init();
  resetAlertButton.init();
  lcd.init();
  displayAdapter.init();
  lcd.clear();

}

void loop() {

  globalSystem.updateEvents();
  globalSystem.FSM_GloablSystem();
  globalSystem.updateActions();
  
}

