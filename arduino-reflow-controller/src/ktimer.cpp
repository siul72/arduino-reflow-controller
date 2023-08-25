#include "ktimer.h"

Ktimer::Ktimer(int TimerPin)
{
    timer_pin = TimerPin;
    on_time_ms = 200;
    off_time_ms  = 2000;
    total_time_ms = on_time_ms + off_time_ms;
    current_output = LOW;  
    timer_enabled = false; 
}

void Ktimer::setup()
{
    pinMode(timer_pin, OUTPUT);
    digitalWrite(timer_pin, LOW);
     
}

void Ktimer::loop()
{
    unsigned long current_millis = millis();
    if (current_millis - previous_millis >= interval) {
      //Serial.println("process timer loop");
      previous_millis = current_millis;
      if (timer_enabled){
        //check if need to be on
        current_time = current_time + interval;
        if (current_output == HIGH) {
            if (current_time >= on_time_ms){
                current_output = LOW;
                Serial.println("TIMER OFF");
                digitalWrite(timer_pin, current_output);
            }
        } else if (current_time >= off_time_ms){
            current_output = HIGH;
            current_time = 0;
            Serial.println("TIMER ON");
            digitalWrite(timer_pin, current_output);
            
        }
     }

    }
}

void Ktimer::start()
{
    timer_enabled = true;
    current_output = HIGH; 
    digitalWrite(timer_pin, current_output);
    current_time = 0;
    Serial.println("TIMER START ON");
}

void Ktimer::stop()
{
    timer_enabled = false;
    digitalWrite(timer_pin, LOW);

}

void Ktimer::setOnTime(int time)
{
    on_time_ms = time;
}

void Ktimer::setOffTime(int time)
{
    off_time_ms = time;
}

void Ktimer::resetCounters()
{
}
