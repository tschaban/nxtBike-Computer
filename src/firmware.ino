
#include <LED.h>
#include <Nextion.h>
#include <WIFI.h>
#include <dataAccess.h>
#include <screen.h>
#include <speedometer.h>
#include <voltage.h>
#include <webServer.h>
#ifdef DEBUG
#include <Streaming.h>
#endif

DATA Data;
SPEEDOMETER SpeedoMeter;
SCREEN Screen;
LED Led;
WIFI AccessPoint;
WEBSERVER WebServer;

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

void speedometerEnabled(boolean enabled) {
  if (enabled) {
    attachInterrupt(digitalPinToInterrupt(SpeedoMeter.configuration.GPIO),
                    newImpulse, RISING);
  } else {
    detachInterrupt(digitalPinToInterrupt(
        digitalPinToInterrupt(SpeedoMeter.configuration.GPIO)));
  }
}

/* VOLTAGE ________________________________ */
VOLTAGE Voltage;

/* SAVING DATA ____________________________ */
uint32_t eventSpeedometterSaveCounter = millis();

/* BACKUP DATA ____________________________ */
uint32_t eventSpeedometterDataArchiveRestoreCounter = millis();

/* used to show time to save at spash screen */
char timeToSaveText[4];
uint8_t timeToSaveCounter = 0;
uint8_t _timeToSaveCounter = 0;
int16_t _timeToSaveCounterReal = 0;
double lastSavedTotalSpeed;

/* SETUP _________________________________ */

void setup() {
  Serial.begin(115200);
  delay(10);

  /* Loading default data */

  if (LittleFS.begin()) {
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

  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.handle("/debug", handleDebug);
  WebServer.handleFirmwareUpgrade("/upgrade", handleHTTPRequests, handleUpload);
  WebServer.begin();

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
  WebServer.listener();
}
