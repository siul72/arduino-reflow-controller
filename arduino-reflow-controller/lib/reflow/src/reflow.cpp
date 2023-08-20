 
#include "reflow.h"

Reflow::Reflow(){

    duty = 0;
    fanState = LOW;
    
    totalDuration = 0;
    zoneDuration = 0;
    futureZone = ReflowZoneEnum::NONE;
    activeZone = ReflowZoneEnum::NONE;
    
    previousMillis = 0;
   
    

}

void Reflow::setup()
{
    pinMode(FAN_CTRL, OUTPUT);
    digitalWrite(FAN_CTRL, fanState);
    Timer1.initialize(30000);
    Timer1.pwm(SSR_CTRL, duty);
    sprintf(buffer, "setup reflow: duty %d", duty);
    Serial.println(buffer);
}

void Reflow::start()
{
}

void Reflow::stop()
{
}

int Reflow::getDuty()
{
    return duty;
}

void Reflow::setDuty(int duty)
{
    if (duty > 1023 or duty < 0) {
      duty = 0;
    }
    sprintf(buffer, "duty %d", duty);
    Serial.println(buffer);
    Timer1.pwm(SSR_CTRL, duty);

    if (duty == 0) {
      if (fanState == LOW) {
        fanState = HIGH;
        digitalWrite(FAN_CTRL, fanState);
        Serial.println("FAN ON");
      }

         
    } else {
      if (fanState == HIGH) {
        fanState = LOW;
        digitalWrite(FAN_CTRL, fanState);
        Serial.println("FAN OFF");
      }
    }
}

void Reflow::incrementDuty(int pduty)
{
  duty = duty + pduty;
  this->setDuty(duty);
}

void Reflow::setCooling(int duration)
{
    zoneDuration = duration;
    currentDuration = 0;
    futureZone = ReflowZoneEnum::COOLING;
}

void Reflow::loop(){
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
      //Serial.println("process reflow loop");
      previousMillis = currentMillis;

      if (futureZone != activeZone) {
        Serial.println("New zone");
        if (futureZone == ReflowZoneEnum::COOLING){
            fanState = HIGH;
            digitalWrite(FAN_CTRL, fanState);
            Serial.println("FAN ON");
            duty = 0;
            Timer1.pwm(SSR_CTRL, 0);

        } else if (futureZone == ReflowZoneEnum::NONE){
            fanState = LOW;
            digitalWrite(FAN_CTRL, fanState);
            Serial.println("FAN OFF");
            duty = 0;
            Timer1.pwm(SSR_CTRL, 0);
            currentDuration = 0;
            zoneDuration = 0;

        }
 

        activeZone = futureZone;
       
      }

       if (activeZone != ReflowZoneEnum::NONE) {

            totalDuration++;
            currentDuration++;

            if(currentDuration>=zoneDuration){
                if (activeZone == ReflowZoneEnum::COOLING){
                    futureZone = ReflowZoneEnum::NONE;
                }
            }

        }

      //print current status
      sprintf(buffer, "Current ZONE %d:CurrentT %d, zoneT %d, TotalT %d, fanstate %d, duty %d ", activeZone, currentDuration, zoneDuration, totalDuration, fanState, duty);
      Serial.println(buffer);
  }
}
 
