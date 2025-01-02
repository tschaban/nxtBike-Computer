/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _WiFi_h
#define _WiFi_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <WiFi.h>
#include <WirelessNetwork-Types.h>
#include <local-config.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

class WirelessNetwork {

private:
  NETWORKType networkConfiguration;
  WiFiClass WirelessConnection;

public:
  WirelessNetwork();

  /* Sets connection parameters and host name. Must be invoked before connect
   * method */
  void begin();
};

#endif
