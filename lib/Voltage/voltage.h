/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _voltage_h
#define _voltage_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <voltage-types.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class VOLTAGE {
public:
  VoltageDataType data;
  VoltageConfigurationType configuration;
  boolean ready = false;

  VOLTAGE();

  void measure(uint32_t measureInterval);
  VoltageDataType get();

private:
  uint32_t startTime;
  uint8_t counterOfSamplings = 0;
  uint16_t temporaryAnalogData = 0;
  uint16_t analogData = 0;
};
#endif
