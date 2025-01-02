#include "WirelessNetwork.h"

WirelessNetwork::WirelessNetwork() {}

void WirelessNetwork::begin() {
  boolean connecting = true;

  WirelessConnection.hostname(networkConfiguration.ssid);
  WirelessConnection.setSleep(WIFI_PS_NONE);

  /* ACCESS POINT */
  if (WIFI_MODE == WIFI_ACCESS_POINT) {
#ifdef DEBUG
    Serial << endl << F("Starting HotSpot: ");
#endif
    IPAddress apIP(192, 168, 5, 1);
    WirelessConnection.mode(WIFI_AP_STA);
    WirelessConnection.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WirelessConnection.softAP(networkConfiguration.ssid);
  }
  if (WIFI_MODE == WIFI_CLIENT) {
/* WIFI */
#ifdef DEBUG
    Serial << endl << F("Connecting to WiFi");
#endif
    WirelessConnection.mode(WIFI_AP_STA);
    WirelessConnection.begin(WIFI_SSID, WIFI_PASSWORD);
    unsigned long timer = millis();
    while (connecting) {
      if (millis() - timer > WIFI_WAIT_TIME * 1000) {
        connecting = false;
#ifdef DEBUG
        Serial << endl << F("Not connected to WiFi");
#endif
        WirelessConnection.disconnect();
      }

      if (WirelessConnection.localIP().toString() != "(IP unset)" ||
          WirelessConnection.status() == WL_CONNECTED) {
#ifdef DEBUG
        Serial << endl << F("Connected to WirelessConnection. IP=") << WirelessConnection.localIP();
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
