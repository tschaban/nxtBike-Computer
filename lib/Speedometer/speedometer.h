/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _speedometer_h
#define _speedometer_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <LED.h>
#include <configuration.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class SPEEDOMETER {

public:
  SpeedometerConfigurationType configuration;
  SpeedometerDataType data;
  unsigned long refreshCounter = 0;

  /* Constructors */
  SPEEDOMETER();

  void addImpulse();
  void calculate();
  void reset();
  void timer(boolean ride = false);

private:
  uint8_t impulseCounter = 0;
  uint32_t firstImpulseTime = 0;
  uint32_t lastImpulseTime = 0;
  uint32_t timerTime = 0;

  uint32_t timer1s = 0;
  uint32_t timerRide1mCounter = 0;

  boolean previousState;
  uint32_t startTime = 0;

  LED Led;

  void addDistance();
};

#endif
