#ifndef DISPLAYADAPTER_H
#define DISPLAYADAPTER_H

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


class DisplayAdapter
{
private:
    LiquidCrystal_I2C &lcd;
    int alertCursorPositionCount = 0;
    bool displayAlert = false;
public:
    DisplayAdapter(LiquidCrystal_I2C &lcd)
        : lcd(lcd)
    {}
    ~DisplayAdapter()
    {}
    void setTemperatureValue(float tempValue){
        //Serial.println(tempValue);
        lcd.setCursor(0, 1);
        if(tempValue > 99.99)    lcd.print("Temp:" + String((int)tempValue));
        else                    lcd.print("Temp:" + String((int)tempValue) + " ");
        
        
    }
    void setBatteryLevel(float batteryLevel){
        lcd.setCursor(0,2);
        if((int)batteryLevel <= 9) lcd.print("Bat: " + String((int)batteryLevel));
        else if((int)batteryLevel > 9) lcd.print("Bat:" + String((int)batteryLevel));
        
    }
    void setReserveLevel(String ReserveLevel){
        lcd.setCursor(10,2);
        lcd.print("Reser:" + ReserveLevel + " ");
    }
    void setMode(String mode){
        if (!displayAlert)
        {
            //Serial.println(mode);
            //lcd.clear();
            lcd.setCursor(5, 0);
            lcd.print("Mode: " + mode + "  ");
        }
    }
    
    void setAlert(String alertType){
        
        if (!(alertCursorPositionCount >= 3)) alertCursorPositionCount++;
        if (alertCursorPositionCount == 3) alertCursorPositionCount = -3;
        
        lcd.setCursor(0 + alertCursorPositionCount, 3);
        if (alertType == "battery")
        {
        lcd.print("   BATTERY LOW !!!       ");
        }else if (alertType == "reserve")
        {
        lcd.print("   RESERVE EMPTY !!!        ");
        }
        else if (alertType == "temperature")
        {
        lcd.print("   TEMPERATURE HIGH !!!     ");
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