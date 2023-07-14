#include <Arduino.h>

String urldecode(String str);
unsigned char h2int(char c);
bool get_token(String &from, String &to, uint8_t index, char separator);
