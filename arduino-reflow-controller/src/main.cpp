#include <Arduino.h>

#include <TM1650.h>
#include <ezButton.h>
#include <reflow.h>
#include "max6675.h"


#define START 9
#define STOP 8


#define thermoDO 4
#define thermoCS 5 
#define thermoCLK 6
 
char buffer[128], szF[6];
int ledState = HIGH;
const long interval = 1000; 
unsigned long previousMillis = 0;

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
TM1650 display;
Reflow myReflow;
ezButton button_start(START);
ezButton button_stop(STOP);

void setup() {
  
  pinMode(START, INPUT); // start button
  pinMode(STOP, INPUT); // stop button
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(9600);
 
  button_start.setDebounceTime(25);
  button_stop.setDebounceTime(25);
  
  
  Serial.println("Ksoft Flow Control Start!");
  Wire.begin(); //Join the bus as master
  display.init();
  display.displayOn();
  char line[] = "----";
  display.displayString(line);
  //display.setBrightnessGradually(TM1650_MAX_BRIGHT); 

  myReflow.setup();
  delay(500);
}

  void displayTemperature(float temperature) {
    dtostrf(temperature, 4, 1, szF );
    sprintf(buffer, "%s", szF);
    Serial.println(buffer);
    display.displayString(buffer);
}
 

void loop() {

  button_start.loop();  
  button_stop.loop();
  myReflow.loop();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      // set the LED with the ledState of the variable:
      digitalWrite(LED_BUILTIN, ledState);
  }
    
  if(button_start.isPressed()){
      Serial.println("BUTTON START PRESSED");
      myReflow.incrementDuty();
    }
  
  if(button_stop.isPressed()){
      Serial.println("BUTTON STOP PRESSED");
      myReflow.setCooling(120);
      
  }
  
  //float temp = thermocouple.readCelsius();
  //displayTemperature(temp);
 
}

