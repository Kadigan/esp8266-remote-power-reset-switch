#include "defines.h"
#include "time.h"
#include "utils.h"
#include "webserver.h"
#include "actions.h"

// password for the web interface
const char *webPassword = "";

// DNS name this unit should report back
const char *dnsName = "";

BearSSL::ESP8266WebServerSecure serverHTTPS(443);
         ESP8266WebServer       serverHTTP(80);

void webServerSetup(){
  serverHTTP.on("/", secureRedirect);
  serverHTTP.begin();

  serverHTTPS.getServer().setRSACert(new BearSSL::X509List(serverCert), new BearSSL::PrivateKey(serverKey));

  serverHTTPS.on("/", showInfoPage);
  serverHTTPS.onNotFound(showInfoPage);

  // reset
  serverHTTPS.on("/reset",        processSwitchWebpage_RESET);
  serverHTTPS.on("/reset/",       processSwitchWebpage_RESET);

  // power/short
  serverHTTPS.on("/power",        processSwitchWebpage_POWER);
  serverHTTPS.on("/power/",       processSwitchWebpage_POWER);
  serverHTTPS.on("/power/short",  processSwitchWebpage_POWER);
  serverHTTPS.on("/power/short/", processSwitchWebpage_POWER);

  // power/long (5s hold)
  serverHTTPS.on("/power/long",   processSwitchWebpage_POWER_LONG);
  serverHTTPS.on("/power/long/",  processSwitchWebpage_POWER_LONG);

  serverHTTPS.begin();
}

void secureRedirect() {
  serverHTTP.sendHeader("Location", String("https://") + dnsName, true);
  serverHTTP.send(301, "text/plain", "");
}

bool passwordMatches(){
  if ( serverHTTPS.hasArg("plain") ){
    String receivedPassword = urldecode(String(serverHTTPS.arg("plain")).substring(9));
    if ( receivedPassword == String(webPassword) )
      return true;
  }
  return false;
}

String pageHeader(){
  return String("<!DOCTYPE html><html lang=\"en\">"
  "  <head>"
  "    <meta charset=\"utf-8\">"
  "    <title>ESP8266 power management</title>"
  "    <style>"
  "      body {"
  "        padding: 10px 20px;"
  "        font-family: Apolonia, Arial, Sans-serif;"
  "        background: #eee;"
  "      }"

  "      label, input {"
  "        margin: 10px 0px 0px 20px;"
  "      }"

  "      input {"
  "        border-radius: 10px;"
  "        border: 1px solid #aaa;"
  "      }"

  "      input[type=password] {"
  "        padding: 3px 7px;"
  "        background: #eee;"
  "      }"

  "      input[type=submit] {"
  "        margin-top: 30px;"
  "        padding: 10px 20px;"
  "        cursor: pointer;"
  "        font-weight: bold;"
  "        transition: all 0.4s;"
  "      }"

  "      input[type=submit]:hover {"
  "        background: #fff;"
  "        color: #000;"
  "        transition: all 0.4s;"
  "      }"
  "    </style>"
  "  </head>"
  "<body>");
}

void processSwitchWebpage_RESET(){
  String content = pageHeader();
  content += "<h1>Reset button</h1>";

  if ( passwordMatches() )
      pushSwitchReset();

  // form body
  content += "<form method=\"post\">";
  content += "<label for=\"password\">Password:</label><br />";
  content += "<input type=\"password\" id=\"password\" name=\"password\"><br />";
  content += "<input type=\"submit\" value=\"RESET\">";
  content += "</form>";

  // end HTML
  content += "</body></html>";
  serverHTTPS.send(200, "text/html", content);
}

void processSwitchWebpage_POWER(){
  String content = pageHeader();
  content += "<h1>Power button / short</h1>";

  if ( passwordMatches() )
      pushSwitchPower();

  // form body
  content += "<form method=\"post\">";
  content += "<label for=\"password\">Password:</label><br />";
  content += "<input type=\"password\" id=\"password\" name=\"password\"><br />";
  content += "<input type=\"submit\" value=\"POWER\">";
  content += "</form>";

  // end HTML
  content += "</body></html>";
  serverHTTPS.send(200, "text/html", content);
}

void processSwitchWebpage_POWER_LONG(){
  String content = pageHeader();
  content += "<h1>Power button / long (5s)</h1>";

  if ( passwordMatches() )
      pushSwitchPowerLong();

  // form body
  content += "<form method=\"post\">";
  content += "<label for=\"password\">Password:</label><br />";
  content += "<input type=\"password\" id=\"password\" name=\"password\"><br />";
  content += "<input type=\"submit\" value=\"POWER HOLD (5s)\">";
  content += "</form>";

  // end HTML
  content += "</body></html>";
  serverHTTPS.send(200, "text/html", content);
}

void showInfoPage() {
  // begin content
  String content = "{";
  char s[255];

  sprintf(s, "\"hostname\":\"%s\",\"version\":\"%s\"", dnsName, VERSION);
  content += s;

  struct tm timeinfo; // to get the current time
  if(getCurrentTimeInLocalTimezone(&timeinfo)){
    sprintf(s, ",\"time\":\"%d-%02d-%02dT%02d:%02d:%02d+%02d:00\"", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                                                    timeinfo.tm_hour,        timeinfo.tm_min,     timeinfo.tm_sec,
                                                                    timeinfo.tm_isdst + 1);
    content += s;
    
    localiseTime(&startTime, &timeinfo);
    sprintf(s, ",\"start-time\":\"%d-%02d-%02dT%02d:%02d:%02d+%02d:00\"", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                                                          timeinfo.tm_hour,        timeinfo.tm_min,     timeinfo.tm_sec,
                                                                          timeinfo.tm_isdst + 1);
    content += s;

    localiseTime(&connectTime, &timeinfo);
    sprintf(s, ",\"last-connected-time\":\"%d-%02d-%02dT%02d:%02d:%02d+%02d:00\"", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                                                                                   timeinfo.tm_hour,        timeinfo.tm_min,     timeinfo.tm_sec,
                                                                                   timeinfo.tm_isdst + 1);
    content += s;
  } else {
    content += ",\"time\":null,\"start-time\":null,\"last-connected-time\":null";
  }

  // end content
  content += "}";
  serverHTTPS.send(200, "application/json", content);
}
