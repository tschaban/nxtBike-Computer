/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _screen_h
#define _screen_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <configuration.h>

#include <Nextion.h>
#include <configuration.h>
#include <dataAccess.h>
#include <voltage-types.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class SCREEN {
public:
  SCREEN();
  void begin();
  void set(uint8_t id);
  void change();
  void changeTheme();

  void refreshS1Status(const char *);
  //  void refreshScreenS1Speed(double speed);
  //  void refreshScreenS1Voltage(VoltageDataType *);
  void refreshScreenS2(SpeedometerDataType *);
  void refreshScreenS2(VoltageDataType *);

  void refreshScreenS3(SpeedometerDataType *);

  void showSyncStatus(uint8_t status = SYNC_COMPLETED);

  uint8_t getActiveScreenID();

  boolean isPressed();
  boolean isPressed2();
  boolean isPressed5();

  void listener();

private:
  LCDScreenConfigurationType configuration;
  LCDScreenCurrentConfigurationType currentScreenConfiguration;

  DATA Data;

  uint8_t currentSyncDataStatus = SYNC_WAITING_TO_SYNC;

  /* Button related */
  boolean state;
  boolean previousState; // Actually this stores current switch state
  unsigned long startTime = 0;
  boolean pressed = false; // It's set to true once button pressed physically
  boolean _pressed = false;
  boolean pressed4LessThan2Seconds = false;
  boolean _pressed4LessThan2Seconds = false;
  boolean pressed42Seconds = false;
  boolean _pressed42Seconds = false;
  boolean pressed45Seconds = false;
  boolean _pressed45Seconds = false;

  NexText S1Status = NexText(0, 1, "S1Status");

  NexVar S2MaxSpeed = NexVar(1, 1, "S2MaxSpeed");
  NexVar S2AvgSpeed = NexVar(1, 2, "S2AvgSpeed");
  NexVar S2Minutes = NexVar(1, 3, "S2Minutes");
  NexVar S2DistTotal = NexVar(1, 4, "S2DistTotal");
  NexVar S2BatLevel = NexVar(1, 5, "S2BatLevel");
  NexVar S2Speed = NexVar(1, 6, "S2Speed");
  NexVar S2BatVoltage = NexVar(1, 8, "S2BatVolt");
  NexVar S2DistTrip = NexVar(1, 9, "S2DistTrip");

  NexProgressBar S2SpeedProg = NexProgressBar(1, 7, "S2SpeedProg");
};
#endif
