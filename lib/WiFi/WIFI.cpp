#include "WIFI.h"

WIFI::WIFI() {}

void WIFI::begin() {
  boolean connecting = true;

  WiFi.hostname(networkConfiguration.ssid);
  WiFi.setSleepMode(WIFI_NONE_SLEEP);

  /* ACCESS POINT */
  if (WIFI_MODE == WIFI_ACCESS_POINT) {
#ifdef DEBUG
    Serial << endl << F("Starting HotSpot: ");
#endif
    IPAddress apIP(192, 168, 5, 1);
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(networkConfiguration.ssid);
  }
  if (WIFI_MODE == WIFI_CLIENT) {
/* WIFI */
#ifdef DEBUG
    Serial << endl << F("Connecting to WiFi");
#endif
    WiFi.mode(WIFI_STA);
    WiFi.begin("POLICJA: Posterunek", "1231231234");
    unsigned long timer = millis();
    while (connecting) {
      if (millis() - timer > WIFI_WAIT_TIME * 1000) {
        connecting = false;
#ifdef DEBUG
        Serial << endl << F("Not connected to WiFi");
#endif
        WiFi.disconnect();
      }

      if (WiFi.localIP().toString() != "(IP unset)" ||
          WiFi.status() == WL_CONNECTED) {
#ifdef DEBUG
        Serial << endl << F("Connected to WiFi. IP=") << WiFi.localIP();
#endif
        connecting = false;
      } else {
#ifdef DEBUG
        Serial << F(".");
#endif
      }

      yield();
    }

#ifdef DEBUG
    Serial << endl << F(" connected");
#endif
  }
}
