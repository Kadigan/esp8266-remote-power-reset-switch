#include <Arduino.h>
#include "defines.h"
#include "actions.h"

void pushSwitchReset(){
  ledON();
  digitalWrite(pinReset, HIGH);
  delay(200);
  digitalWrite(pinReset, LOW);
  ledOFF();
}

void pushSwitchPower(){
  ledON();
  digitalWrite(pinPower, HIGH);
  delay(200);
  digitalWrite(pinPower, LOW);
  ledOFF();
}

void pushSwitchPowerLong(){
  ledON();
  digitalWrite(pinPower, HIGH);
  delay(5000);
  digitalWrite(pinPower, LOW);
  ledOFF();
}

void ledON(){
  digitalWrite(pinLED, LEDHI);
}

void ledOFF(){
  digitalWrite(pinLED, LEDLO);
}
