#ifndef FIRMWARE_H
#define FIRMWARE_H


#include <Nextion.h>
#include <WirelessNetwork.h>
#include <LED.h>
#include <dataAccess.h>
#include <screen.h>
#include <speedometer.h>
#include <voltage.h>
#include <HttpServer.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

DATA Data;
SPEEDOMETER SpeedoMeter;
SCREEN Screen;
LED Led;
WirelessNetwork AccessPoint;
HTTPSERVER HttpServer;

/* SPEEDOMETER  _________________________________ */
void ICACHE_RAM_ATTR newImpulse() {
  static uint32_t lastInterruptionTime = 0;
  uint32_t interruptionTime = millis();
  if (interruptionTime - lastInterruptionTime >
      (uint32_t)SpeedoMeter.configuration.signalBouncing) {
    SpeedoMeter.addImpulse();
  }
  lastInterruptionTime = interruptionTime;
}

void speedometerEnabled(boolean enabled);


VOLTAGE Voltage;
uint32_t eventSpeedometterSaveCounter = millis();
uint32_t eventSpeedometterDataArchiveRestoreCounter = millis();

/* used to show time to save at spash screen */
char timeToSaveText[4];
uint8_t timeToSaveCounter = 0;
uint8_t _timeToSaveCounter = 0;
int16_t _timeToSaveCounterReal = 0;
double lastSavedTotalSpeed;

#endif