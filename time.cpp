#include "time.h"

time_t connectTime; // last WiFi connection time
time_t startTime;   // startup time
unsigned long startTimeRaw;

bool timeConfigured = false;

bool localiseTime(time_t *time, struct tm *info){
  uint32_t start = millis();
  while( (millis() - start) <= 500 ){
    localtime_r(time, info); // localize into local TZ
    if( info->tm_year > (2022 - 1900) ) // check if year is sane
      return true;
    delay(10);
  }
  return false;
}

bool getCurrentTimeInLocalTimezone(struct tm *info){
  time_t now;
  time(&now);
  return localiseTime(&now, info);
}

bool handleTimeConfiguration(){
  struct tm timeinfo;
  configTime(0, 0, "pool.ntp.org", "time.nist.gov", "time.windows.com");
  if(getCurrentTimeInLocalTimezone(&timeinfo)){
    // Europe/Warsaw = CET-1CEST,M3.5.0,M10.5.0/3
    Serial.println("Timezone set to CET-1CEST,M3.5.0,M10.5.0/3 (Europe/Warsaw).");
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
    tzset();

    time(&startTime);
    startTime -= (millis() - startTimeRaw) / 1000;

    timeConfigured = true;
    return true;
  }

  return false;
}
