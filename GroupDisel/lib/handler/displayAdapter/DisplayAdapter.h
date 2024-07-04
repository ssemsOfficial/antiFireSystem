#ifndef DISPLAYADAPTER_H
#define DISPLAYADAPTER_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


// Define the custom characters
byte emptyBatteryChar[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};

byte char20Battery[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111
};

byte char40Battery[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111
};

byte char60Battery[] = {
  B01110,
  B11011,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111
};

byte char80Battery[] = {
  B01110,
  B11011,
  B10001,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

byte fullBatteryChar[] = {
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111
};

class DisplayAdapter
{
private:
    LiquidCrystal_I2C &lcd;
    int alertCursorPositionCount = 0;
    bool displayAlert = false;
    //custom char
    
public:
    DisplayAdapter(LiquidCrystal_I2C &lcd)
        : lcd(lcd)
    {}
    ~DisplayAdapter()
    {}
    void init(){
        lcd.createChar(0, emptyBatteryChar);
        lcd.createChar(1, char20Battery);
        lcd.createChar(2, char40Battery);
        lcd.createChar(3, char60Battery);
        lcd.createChar(4, char80Battery);
        lcd.createChar(5, fullBatteryChar);
    }
    void setTemperatureValue(float tempValue){
        lcd.setCursor(0, 1);
        if(tempValue > 99.99)    lcd.print("Temp:" + String((int)tempValue));
        else                    lcd.print("Temp:" + String((int)tempValue) + " ");
        
    }
    void setPressionValue(float pression){
        lcd.setCursor(10, 1);
        if(pression > 99.99)    lcd.print("Press:" + String((int)pression));
        else                    lcd.print("Press:" + String((int)pression) + "    ");
        
    }
    void setBattery1Level(float batteryLevel){
        int level = map((int)batteryLevel,0,12,0,100);
        int charIndex = map(level, 0, 100, 0, 5);
        lcd.setCursor(0,2);
        lcd.print("Bat1:");
        lcd.setCursor(4,2);
        lcd.write(charIndex);
        //if((int)batteryLevel <= 9) lcd.print("Bat: " + String((int)batteryLevel));
        //else if((int)batteryLevel > 9) lcd.print("Bat:" + String((int)batteryLevel));
    }
    void setBattery2Level(float batteryLevel){
        int level = map((int)batteryLevel,0,12,0,100);
        int charIndex = map(level, 0, 100, 0, 5);
        lcd.setCursor(6,2);
        lcd.print("Bat2:");
        lcd.setCursor(10,2);
        lcd.write(charIndex);
        //if((int)batteryLevel <= 9) lcd.print("Bat: " + String((int)batteryLevel));
        //else if((int)batteryLevel > 9) lcd.print("Bat:" + String((int)batteryLevel));
    }
    void setMode(String mode){
        if (!displayAlert)
        {
            //Serial.println(mode);
            //lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Mode: " + mode + " ");
        }
    }
    void setReserveLevel(String ReserveLevel){
        lcd.setCursor(11,0);
        lcd.print("Res:" + ReserveLevel + " ");
    }
    
    void setAlert(String alertType){
        
        if (!(alertCursorPositionCount >= 3)) alertCursorPositionCount++;
        if (alertCursorPositionCount == 3) alertCursorPositionCount = 0;
        
        lcd.setCursor(0 + alertCursorPositionCount, 3);
        if (alertType == "battery")
        {
          lcd.print("BATTERY LOW !!!       ");
        }else if (alertType == "reserve")
        {
          lcd.print("   RESERVE EMPTY !!!        ");
        }
        else if (alertType == "temperature")
        {
          lcd.print("   TEMPERATURE HIGH !!!     ");
        }
        else if (alertType == "water pression")
        {
          lcd.print("   water pression low !!!     ");
        }
        
              
    }
    void setNoAlert(){
        lcd.setCursor(0, 3);
        lcd.print("                    ");
    }
    void setDisplayAnAlert(bool displayAnAlert){
        displayAlert = displayAnAlert;
    }
    
};

#endif