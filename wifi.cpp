#include <ESP8266WiFi.h>
#include "defines.h"
#include "wifi.h"
#include "time.h"
#include "webserver.h"
#include "actions.h"

const char *ssid     = "";
const char *password = "";

bool doConnectToWiFi(byte timeout){
  WiFi.disconnect();
  WiFi.begin(ssid, password);
  for (byte i = 0; i < timeout; i++) {
    if (WiFi.status() == WL_CONNECTED) {
      return true;
    }

    // these should sump up to 1 second
    ledON();
    delay(50);
    ledOFF();
    delay(50);
    ledON();
    delay(50);
    ledOFF();
    delay(850);

    Serial.print(".");
  }
  Serial.println(" ");

  return false;
}

bool isWiFiConnected(){
  return (WiFi.status() == WL_CONNECTED ? true : false);
}

void handleWiFiConnection(){
  if ( WiFi.status() != WL_CONNECTED ){
    char buffer[255];

    // blink the LED to indicate that we're about to make a connection to WiFi
    ledOFF();
    delay(1000);
    ledON();
    delay(300);
    ledOFF();
    delay(300);
    ledON();
    delay(300);
    ledOFF();
    delay(300);
    ledON();
    delay(500);
    ledOFF();

    sprintf(buffer, "\n\nConnecting (120s timeout) to %s", ssid);
    Serial.println(buffer);

    if (!doConnectToWiFi(30)) {
      for(uint8 i = 0; i < 10; i++){
        ledON();
        delay(100);
        ledOFF();
        delay(100);
      }

      delay(2000);
      return;
    }

    Serial.println("\n\nConnected to WiFi");
    sprintf(buffer, "Server can be accessed at https://%s (IP: %s)", dnsName, WiFi.localIP());
    Serial.println(buffer);  

    ledOFF();
    time(&connectTime); // capture last connected time
  }
}
