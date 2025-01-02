/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "speedometer.h"

SPEEDOMETER::SPEEDOMETER() {
  timerTime = millis();
  refreshCounter = timerTime;
  timer1s = millis();
  pinMode(configuration.GPIO, INPUT_PULLUP);
  previousState = digitalRead(configuration.GPIO);
}

void SPEEDOMETER::addImpulse() {
  impulseCounter++;
  if (impulseCounter == 1) {
    firstImpulseTime = millis();
  } else {
    lastImpulseTime = millis();
  }
  #ifdef DEBUG
    Serial << endl
           << F("Number of impulses: ") << (impulseCounter)
           << F(", within: ") << ((millis() - firstImpulseTime) / 1000) << F("s");
  #endif
  addDistance();
}

void SPEEDOMETER::calculate() {

  if (lastImpulseTime > 0 && impulseCounter > 1) {
    uint8_t _impulseCounter = impulseCounter;
    uint32_t _lastImpulseTime = lastImpulseTime;
    uint32_t _firstImpulseTime = firstImpulseTime;

    impulseCounter = 0;
    lastImpulseTime = 0;

    double distanceFromLastCheck =
        (double)((_impulseCounter - 1) * configuration.wheelSize) / 100000;
    double measureTime =
        (double)(_lastImpulseTime - _firstImpulseTime) / 3600000;

    data.speed.current = distanceFromLastCheck / measureTime;
    configuration.refreshFrequency =
        SPEEDOMETER_NUMBER_OF_IMPULSES_EXPTECTED *
        ((_lastImpulseTime - _firstImpulseTime) / _impulseCounter);

    if (configuration.refreshFrequency > SPEEDOMETER_MAX_SPEED_REFRESH_TIME) {
      configuration.refreshFrequency = SPEEDOMETER_MAX_SPEED_REFRESH_TIME;
    }

  } else if (lastImpulseTime == 0 && impulseCounter == 1) {
    impulseCounter = 0;
  } else {
    data.speed.current = 0;
    configuration.refreshFrequency = SPEEDOMETER_MAX_SPEED_REFRESH_TIME;
  }

  if (data.speed.current > data.speed.max) {
    data.speed.max = data.speed.current;
  }

  data.speed.maxPercent = (data.speed.current / SPEEDOMETER_MAX_SPEED) * 100;
  /*
  #ifdef DEBUG
    Serial << endl
           << F("Speed current: ") << data.speed.current
           << F("km/h, max: ") << data.speed.max << F("km/h, "
           << F("Refresh time: ") << configuration.refreshFrequency;
  #endif
  */
}

void SPEEDOMETER::addDistance() {
  data.distance.current += configuration.wheelSize / 100000;
  data.distance.total += configuration.wheelSize / 100000;
  /*
  #ifdef DEBUG
    Serial << endl
           << F("Distance: current: ") << data.distance.current
           << F("km, total: ") << data.distance.total << F("km";
  #endif
  */
}

void SPEEDOMETER::reset() {
  data.distance.current = 0;
  data.timer = 0;
  data.timeInMove = 0;
  data.speed.avarage = 0;
  data.speed.max = 0;
}

void SPEEDOMETER::timer(boolean ride) {
  if (millis() - timerTime >= 60000) {
    data.timer++;
    timerTime = millis();
    if (data.timer > 0) {
      /*
#ifdef DEBUG
      Serial << endl
             << F("Average speed: distance ") << data.distance.current
             << F(" minutes : ") << data.timer
             << F(" time: ") << (double)((double)data.timer / 60) << F("h";
#endif
*/
      data.speed.avarage = data.timeInMove > 0
                               ? (double)data.distance.current /
                                     (double)((double)data.timeInMove / 60 / 60)
                               : 0;
      /*
#ifdef DEBUG
Serial << F(" speed ") << data.speed.avarage << F("km/h";
#endif
*/
    }
  }

  if (ride) {
    if (millis() - timer1s >= 1000) {
      timer1s = millis();
      data.timeInMove++;
    }
  } else {
    timer1s = millis();
  }
}
