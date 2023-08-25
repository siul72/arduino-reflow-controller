 
#include "reflow.h"

Reflow::Reflow()
{

    duty = 0;
    fanState = LOW;
    
    totalDuration = 0;
    zoneDuration = 0;
    futureZone = ReflowZoneEnum::NONE;
    activeZone = ReflowZoneEnum::NONE;
    
    previousMillis = 0;
    my_timer = new Ktimer(SSR_CTRL);

}

void Reflow::setup()
{
    pinMode(FAN_CTRL, OUTPUT);
    digitalWrite(FAN_CTRL, fanState);
    my_timer->setup();
    sprintf(buffer, "setup reflow");
    Serial.println(buffer);
}

void Reflow::start()
{
}

void Reflow::stop()
{
}

void Reflow::setNextZone()
{
  switch (activeZone)
  {
    case ReflowZoneEnum::NONE:
      futureZone = ReflowZoneEnum::PRE_START;
    break;

    case ReflowZoneEnum::PRE_START:
      futureZone = ReflowZoneEnum::PRE_START_STEADY;
    break;

    case ReflowZoneEnum::PRE_START_STEADY:
      futureZone = ReflowZoneEnum::PRE_HEAT;
    break;

    case ReflowZoneEnum::PRE_HEAT:
      futureZone = ReflowZoneEnum::SOAK;
    break;

    case ReflowZoneEnum::SOAK:
      futureZone = ReflowZoneEnum::REFLOW;
    break;

    case ReflowZoneEnum::REFLOW:
      futureZone = ReflowZoneEnum::REFLOW_STEADY;
    break;
    
    default:
    break;
  }
}
 

void Reflow::setCooling()
{
     
    futureZone = ReflowZoneEnum::COOLING;
}

void Reflow::loop(){
  my_timer->loop();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) 
  {
      //Serial.println("process reflow loop");
      previousMillis = currentMillis;

      if (futureZone != activeZone) 
      {
        Serial.println("New Future Zone");
        switch (futureZone)
        {
          case ReflowZoneEnum::PRE_START:
            fanState = LOW;
            digitalWrite(FAN_CTRL, fanState);
            my_timer->setOnTime(200);
            my_timer->setOffTime(2000);
            my_timer->start();
            zoneDuration = 60;
            currentDuration = 0;
            Serial.println("Timer Started");
          break;
          case ReflowZoneEnum::PRE_START_STEADY:
            zoneDuration = 30;
            currentDuration = 0;
            my_timer->stop();
          break;
          case ReflowZoneEnum::PRE_HEAT:
            my_timer->stop();
            my_timer->setOnTime(350);
            my_timer->setOffTime(1000);
            my_timer->start();
            Serial.println("Timer Started for PRE_HEAT");
            zoneDuration = 60;
            currentDuration = 0;
            
          break;
          case ReflowZoneEnum::SOAK:
            my_timer->stop();
            my_timer->setOnTime(300);
            my_timer->setOffTime(3000);
            my_timer->start();
            Serial.println("Timer Started for SOAK");
            zoneDuration = 120;
            currentDuration = 0;
             
          break;

          case ReflowZoneEnum::REFLOW:
            my_timer->stop();
            my_timer->setOnTime(2000);
            my_timer->setOffTime(1000);
            my_timer->start();
            Serial.println("Timer Started for REFLOW");
            zoneDuration = 20;
            currentDuration = 0;
             
          break;
           case ReflowZoneEnum::REFLOW_STEADY:
            my_timer->stop();
            Serial.println("Timer Started for REFLOW_STEADY");
            zoneDuration = 20;
            currentDuration = 0;
   
          break;
          case ReflowZoneEnum::COOLING:
            fanState = HIGH;
            digitalWrite(FAN_CTRL, fanState);
            Serial.println("FAN ON");
            zoneDuration = 600;
            currentDuration = 0;
            my_timer->stop();
          break;
          case ReflowZoneEnum::NONE:
            fanState = LOW;
            digitalWrite(FAN_CTRL, fanState);
            Serial.println("FAN OFF");
            my_timer->stop();
            currentDuration = 0;
            zoneDuration = 0;
          break;

          default:
            Serial.println(" FUTURE ZONE UNKNOWN");
          break;
        }
        
        activeZone = futureZone;
       
      }

       if (activeZone != ReflowZoneEnum::NONE) {

            totalDuration++;
            currentDuration++;

            if(currentDuration>=zoneDuration){
                
                if (activeZone == ReflowZoneEnum::PRE_START){
                  futureZone = ReflowZoneEnum::PRE_START_STEADY;
                }

                if (activeZone == ReflowZoneEnum::PRE_START_STEADY){
                  futureZone = ReflowZoneEnum::PRE_HEAT;
                }

                if (activeZone == ReflowZoneEnum::PRE_HEAT){
                  futureZone = ReflowZoneEnum::SOAK;
                }

                if (activeZone == ReflowZoneEnum::SOAK){
                  futureZone = ReflowZoneEnum::REFLOW;
                }

                if (activeZone == ReflowZoneEnum::REFLOW){
                  futureZone = ReflowZoneEnum::REFLOW_STEADY;
                }

                if (activeZone == ReflowZoneEnum::REFLOW_STEADY){
                  futureZone = ReflowZoneEnum::COOLING;
                }
                
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
 
