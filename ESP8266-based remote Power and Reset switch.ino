#include <ESP8266WiFi.h>
#include "defines.h"
#include "time.h"
#include "actions.h"
#include "wifi.h"
#include "webserver.h"

// ========================================================================================================
// ========================================================================================================
// ========================================================================================================
// developed on: NodeMCU 1.0 (ESP-12E Module)
// author: Kadigan, kadigan.ksb@gmail.com, 2023/07

// the setup function runs once when you press reset or power the board
void setup() {
  char buffer[255];
  startTimeRaw = millis();

  Serial.begin(9600);
  Serial.println("setup()");

  // set modes
  pinMode(pinPower,  OUTPUT);
  pinMode(pinReset,  OUTPUT);
  pinMode(pinLED,    OUTPUT);

  // set outputs
  digitalWrite(pinPower, LOW);
  digitalWrite(pinReset, LOW);

  // configure the webserver
  webServerSetup();

  // light the led up to show we've just started up
  ledON();
  delay(2000);
  ledOFF();
  delay(2000);
}

unsigned long statusLedBlinkInterval       = 10000; // in ms, 10000 = 10 seconds
unsigned long statusLedBlinkPreviousMillis = 0;

void loop() {
  unsigned long currentMillis = millis();

  if ( isWiFiConnected() ){
    serverHTTP.handleClient();
    serverHTTPS.handleClient();

    if ( currentMillis - statusLedBlinkPreviousMillis > statusLedBlinkInterval ){
      ledON();
      delay(10);
      ledOFF();
      delay(30);
      statusLedBlinkPreviousMillis = millis(); // save the LATEST time, so that we check time+interval to LAST CHECK, not a set timer like the blinker

      if ( !timeConfigured ){
        if ( handleTimeConfiguration() ){
          for (uint8_t i = 0; i < 3; i++){
            ledON();
            delay(50);
            ledOFF();
            delay(50);
          }
        }
      }
    }
  } else {
    handleWiFiConnection();
  }
}
