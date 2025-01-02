/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _dataAccess_h
#define _dataAccess_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <configuration.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class DATA {
private:
  uint32_t eventSpeedometterSaveCounter = 0;

  void createSpeedometterFile();
  void createCurrentScreenFile();

public:
  DATA();
  boolean saving = false;

  SpeedometerDataType getSpeedometterData();
  void saveSpeedometterData(SpeedometerDataType *);

  void createdBackup();
  void restoreBackup();

  LCDScreenCurrentConfigurationType getCurrentScreen();
  void saveCurrentScreen(LCDScreenCurrentConfigurationType *);

  boolean eventSpeedometterSave();

  void formatFileSystem();
};
#endif
