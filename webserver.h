#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>

void secureRedirect();
bool passwordMatches();
void processSwitchWebpage_RESET();
void processSwitchWebpage_POWER();
void processSwitchWebpage_POWER_LONG();
void showInfoPage();
void webServerSetup();

static const char serverCert[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
you know what to put here ;) note: the ESP8266 isn't too fast, so at best use a 1K key pair
-----END CERTIFICATE-----
)EOF";

static const char serverKey[] PROGMEM =  R"EOF(
-----BEGIN PRIVATE KEY-----
see note above
-----END PRIVATE KEY-----
)EOF";

// password for the web interface
extern const char *webPassword;

// DNS name this unit should report back
extern const char *dnsName;

extern esp8266webserver::ESP8266WebServerTemplate<WiFiServer> serverHTTP;
extern esp8266webserver::ESP8266WebServerTemplate<WiFiServerSecure> serverHTTPS;
