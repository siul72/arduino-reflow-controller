#ifndef	REFLOW_H
#define REFLOW_H

#include <Arduino.h>
#include <ktimer.h>
 
#define SSR_CTRL 10 //Arduino Uno 9 or 10
#define FAN_CTRL 11

enum class ReflowZoneEnum{
    NONE=0,
    PRE_START,
    PRE_START_STEADY,
    PRE_HEAT,
    SOAK,
    REFLOW,
    REFLOW_STEADY,
    COOLING,
    MAX
 

};
 


class	Reflow{

    public:
		Reflow(); 
        void setup();
        void loop();
        void start();
        void stop();
         
        void setNextZone(); 
        void setCooling();

    private:
        char buffer[128], szF[6];
        int duty;
        int fanState;
        int totalDuration;
        int zoneDuration;
        String futureStringZone;
        ReflowZoneEnum futureZone;
        ReflowZoneEnum activeZone;
        int currentDuration;
        const long interval = 1000; 
        unsigned long previousMillis;
        Ktimer *my_timer;
};

#endif