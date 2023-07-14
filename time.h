#include <Arduino.h>

bool getCurrentTimeInLocalTimezone(struct tm *info);
bool localiseTime(time_t *time, struct tm *info);
bool handleTimeConfiguration();

extern bool timeConfigured;
extern time_t connectTime; // last WiFi connection time
extern time_t startTime;   // startup time
extern unsigned long startTimeRaw;
