bool doConnectToWiFi(byte timeout = 120);
bool isWiFiConnected();
void handleWiFiConnection();

// WiFi logon credentials
extern const char *ssid;
extern const char *password;
