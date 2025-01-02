
#include "firmware.h"


/* WEBERVER TO REFACTOR */

/* Method handles favicon.ico request */
void handleFavicon() {}
void handleHTTPRequests() { HttpServer.generate(); }
void handleUpload() { HttpServer.generate(true); }
void handleOnNotFound() {
  String page = "<head><meta http-equiv=\"refresh\" content=\"0; "
                "url=http://192.168.5.1/\" /></head><body><p>Opening "
                "configuration site ...</p></body>";
  HttpServer.publishHTML(page);
}
void handleDebug() {
  String page = "";

  page += Voltage.data.percent;
  page += "<br>";
  page += Voltage.data.voltage;

  HttpServer.publishHTML(page);
}



/* END : WEBERVER TO REFACTOR */

/* Refactor */

void speedometerEnabled(boolean enabled) {
  if (enabled) {
    attachInterrupt(digitalPinToInterrupt(SpeedoMeter.configuration.GPIO),
                    newImpulse, RISING);
  } else {
    detachInterrupt(digitalPinToInterrupt(
        digitalPinToInterrupt(SpeedoMeter.configuration.GPIO)));
  }
}


/* End Refactor */



void setup() {
  Serial.begin(115200);
  delay(10);

  /* Loading default data */

  if (LITTLEFS.begin()) {
#ifdef DEBUG
    Serial << endl << F("INFO: File system: mounted");
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: File system: NOT mounted");
  }
#endif

#ifdef DEBUG
  Serial << endl << endl << F("##### BOOTING #####") << endl << endl;
#endif

  Led.on();

  uint8_t lastScreen = Screen.getActiveScreenID();

  nexInit();
  Screen.set(SCREEN_SPLASH);

  AccessPoint.begin();

  pinMode(SpeedoMeter.configuration.GPIO, INPUT_PULLUP);
  speedometerEnabled(true);

  SpeedoMeter.data = Data.getSpeedometterData();
  Screen.refreshScreenS2(&SpeedoMeter.data);

  lastSavedTotalSpeed = SpeedoMeter.data.distance.total;

  HttpServer.handle("/", handleHTTPRequests);
  HttpServer.handle("/favicon.ico", handleFavicon);
  HttpServer.handle("/debug", handleDebug);
  HttpServer.handleFirmwareUpgrade("/upgrade", handleHTTPRequests, handleUpload);
  HttpServer.begin();

  Screen.set(lastScreen);
  Screen.showSyncStatus(SYNC_COMPLETED);

  Led.off();
}

/* LOOP  _________________________________ */

void loop() {

  if (millis() - SpeedoMeter.refreshCounter >
      SpeedoMeter.configuration.refreshFrequency) {
    SpeedoMeter.calculate();
    switch (Screen.getActiveScreenID()) {
    case SCREEN_SPEEDOMETER:
      Screen.refreshScreenS2(&SpeedoMeter.data);
      break;
    case SCREEN_SPEED:
      Screen.refreshScreenS3(&SpeedoMeter.data);
      break;
    case SCREEN_SPLASH:
      break;
    }
    SpeedoMeter.refreshCounter = millis();
  }

  /* Refresh of battery level at Speedometer screen */
  if (Screen.getActiveScreenID() == SCREEN_SPEEDOMETER) {
    Voltage.measure(SpeedoMeter.configuration.refreshBatterFrequency * 1000);
    if (Voltage.ready) {
      Voltage.get();
      Screen.refreshScreenS2(&Voltage.data);
    }
  }

  /* Saving data if speed < SPEEDOMETER_DATA_SAVE_SPEED km/h & interval
   * reached*/
  if (millis() - eventSpeedometterSaveCounter >=
          SPEEDOMETER_DATA_SAVE_FREQUENCY * 1000 &&
      SpeedoMeter.data.speed.current < SPEEDOMETER_DATA_SAVE_SPEED) {
    if (lastSavedTotalSpeed < SpeedoMeter.data.distance.total) {
      if (Screen.getActiveScreenID() == SCREEN_SPLASH) {
        Screen.refreshS1Status("Saving data ...");
      }
      speedometerEnabled(false);
      Data.saveSpeedometterData(&SpeedoMeter.data);
      speedometerEnabled(true);

      lastSavedTotalSpeed = SpeedoMeter.data.distance.total;
    }
    eventSpeedometterSaveCounter = millis();
  } else {
    _timeToSaveCounterReal =
        round((float)(((SPEEDOMETER_DATA_SAVE_FREQUENCY * 1000) -
                       (millis() - eventSpeedometterSaveCounter)) /
                      1000));
    _timeToSaveCounter =
        (_timeToSaveCounterReal >= 0) ? (uint8_t)_timeToSaveCounterReal : 0;

    if (Screen.getActiveScreenID() == SCREEN_SPLASH) {
      if (_timeToSaveCounter != timeToSaveCounter) {
        sprintf(timeToSaveText, "%d", _timeToSaveCounter);
        Screen.refreshS1Status(timeToSaveText);
        timeToSaveCounter = _timeToSaveCounter;
      }
    } else {
      if (lastSavedTotalSpeed < SpeedoMeter.data.distance.total) {
        if (_timeToSaveCounter > 0) {
          Screen.showSyncStatus(SYNC_NEW_DATA);
        } else {
          Screen.showSyncStatus(SYNC_WAITING_TO_SYNC);
        }
      } else {
        Screen.showSyncStatus(SYNC_COMPLETED);
        timeToSaveCounter = _timeToSaveCounter;
      }
    }
  }

  /* Restore / Archive */
  if (millis() - eventSpeedometterDataArchiveRestoreCounter >=
          SPEEDOMETER_BACKUP_RESTORE_TIME * 1000 &&
      SpeedoMeter.data.speed.current < SPEEDOMETER_DATA_SAVE_SPEED) {
    speedometerEnabled(false);
    if (SpeedoMeter.data.distance.total < SPEEDOMETER_DEFAULT_TOTAL_DISTANCE) {
      Data.restoreBackup();
    } else {
      Data.createdBackup();
    }
    speedometerEnabled(true);
  }

  /* Button */
  if (Screen.isPressed()) {
    speedometerEnabled(false);
    Screen.change();
    speedometerEnabled(true);
  } else if (Screen.isPressed2()) {
    speedometerEnabled(false);
    Screen.changeTheme();
    speedometerEnabled(true);
  } else if (Screen.isPressed5()) {
    SpeedoMeter.reset();
  }

  Screen.listener();
  SpeedoMeter.timer(SpeedoMeter.data.speed.current > 0 ? true : false);
  HttpServer.listener();
}
