#ifndef	KTIMER_H
#define KTIMER_H

#include <Arduino.h>
  
class	Ktimer{

    public:
		Ktimer(int TimerPin); 
        void setup();
        void loop();
        void start();
        void stop();
        void setOnTime(int time);
        void setOffTime(int time);
        

    private:
        char buffer[128], szF[6];
        int timer_pin;
        int on_time_ms;
        int off_time_ms;
        int total_time_ms;
        int current_time;
        int current_output;  
        bool timer_enabled;  
        const long interval = 100; 
        unsigned long previous_millis;
        void resetCounters();
};

#endif