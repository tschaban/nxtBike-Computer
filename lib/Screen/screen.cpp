#include "screen.h"

SCREEN::SCREEN() {
  pinMode(configuration.buttonGPIO, INPUT_PULLUP);
  state = digitalRead(configuration.buttonGPIO);
  previousState = state;
}

void SCREEN::begin() {
  currentScreenConfiguration = Data.getCurrentScreen();
#ifdef DEBUG
  Serial << endl
         << F("Setting screen to: ") << currentScreenConfiguration.screenID
         << F("; Theme: ") << currentScreenConfiguration.themeID;
#endif
  set(currentScreenConfiguration.screenID);
}

void SCREEN::change() {
  switch (currentScreenConfiguration.screenID) {
  case SCREEN_SPLASH:
    set(SCREEN_SPEEDOMETER);
    break;
  case SCREEN_SPEEDOMETER:
    set(SCREEN_SPEED);
    break;
  case SCREEN_SPEED:
    set(SCREEN_SPEEDOMETER);
    break;
  }
}

void SCREEN::set(uint8_t id) {
#ifdef DEBUG
  Serial << endl
         << F("Changing screen to: ") << id
         << F("; Theme: ") << currentScreenConfiguration.themeID;
#endif
  switch (id) {
  case SCREEN_SPLASH:
    sendCommand("page S0");
    currentScreenConfiguration.screenID = SCREEN_SPLASH;
    break;
  case SCREEN_SPEEDOMETER:
    if (currentScreenConfiguration.themeID == THEME_NORMAL) {
      sendCommand("page S1");
    } else if (currentScreenConfiguration.themeID == THEME_LIGHT) {
      sendCommand("page S1L");
    } else {
      sendCommand("page S1D");
    }
    currentScreenConfiguration.screenID = SCREEN_SPEEDOMETER;
    break;
  case SCREEN_SPEED:
    if (currentScreenConfiguration.themeID == THEME_NORMAL) {
      sendCommand("page S2");
    } else if (currentScreenConfiguration.themeID == THEME_LIGHT) {
      sendCommand("page S2L");
    } else {
      sendCommand("page S2D");
    }
    currentScreenConfiguration.screenID = SCREEN_SPEED;
    break;
  }
  Data.saveCurrentScreen(&currentScreenConfiguration);
}

void SCREEN::changeTheme() {
  currentScreenConfiguration.themeID++;
  if (currentScreenConfiguration.themeID > 3) {
    currentScreenConfiguration.themeID = 1;
  }
  set(currentScreenConfiguration.screenID);
}

void SCREEN::showSyncStatus(uint8_t status) {
  if (status == currentSyncDataStatus) {
    return;
  } else {
    switch (status) {
    case SYNC_COMPLETED:
      sendCommand("vis S1SyncG,1");
      break;
    case SYNC_NEW_DATA:
      sendCommand("vis S1SyncA,1");
      break;
    case SYNC_WAITING_TO_SYNC:
      sendCommand("vis S1SyncR,1");
      break;
    }

    switch (currentSyncDataStatus) {
    case SYNC_COMPLETED:
      sendCommand("vis S1SyncG,0");
      break;
    case SYNC_NEW_DATA:
      sendCommand("vis S1SyncA,0");
      break;
    case SYNC_WAITING_TO_SYNC:
      sendCommand("vis S1SyncR,0");
      break;
    }
    currentSyncDataStatus = status;
  }
}

/*
void SCREEN::refreshScreenS1Speed(double speed) {
  S1Speed.setValue((uint32_t)round(speed));
}

void SCREEN::refreshScreenS1Voltage(VoltageDataType *data) {

  char result[5]; // Buffer big enough for 7-character float
  dtostrf((float)data->voltage, 2, 1, result);
  S1Voltage.setText(result);
  S1ProgVolt.setValue(data->percent);
}
*/

void SCREEN::refreshS1Status(const char *status) { S1Status.setText(status); }

void SCREEN::refreshScreenS2(SpeedometerDataType *data) {
  S2Speed.setValue((uint32_t)round(data->speed.current));
  S2SpeedProg.setValue(data->speed.maxPercent);
  S2DistTrip.setValue((uint32_t)round(data->distance.current * 100));
  S2DistTotal.setValue((uint32_t)round(data->distance.total));
  S2AvgSpeed.setValue((uint32_t)round(data->speed.avarage));
  S2MaxSpeed.setValue((uint32_t)round(data->speed.max));
  S2Minutes.setValue(data->timer);
}

void SCREEN::refreshScreenS2(VoltageDataType *data) {
  S2BatLevel.setValue((uint32_t)data->percent);
  S2BatVoltage.setValue((uint32_t)trunc(round((float)data->voltage * 10)));
}

void SCREEN::refreshScreenS3(SpeedometerDataType *data) {
  S2Speed.setValue((uint32_t)round(data->speed.current));
  S2SpeedProg.setValue(data->speed.maxPercent);
}

uint8_t SCREEN::getActiveScreenID() {
  return currentScreenConfiguration.screenID;
}

void SCREEN::listener() {
  boolean currentState = digitalRead(configuration.buttonGPIO);

  unsigned long time = millis();

  if (currentState != previousState) { // buttons has been pressed

    if (startTime == 0) { // starting timer. used for switch sensitiveness
      startTime = time;
    }

    if (time - startTime >= configuration.bouncing) {

      if (!_pressed) {
        state = !state;
        _pressed = true;
        pressed = true;
      }

      if (time - startTime >= 3000 && !_pressed45Seconds) {
        _pressed45Seconds = true;
        /*
#ifdef DEBUG
        Serial << endl << F("Pressing button for 5sec";
#endif
*/
      }

      if (time - startTime >= 1000 && !_pressed42Seconds) {
        _pressed42Seconds = true;
/*
#ifdef DEBUG
        Serial << endl << F("Pressing button for 2sec";
#endif
  */    }

if (time - startTime >= configuration.bouncing && !_pressed4LessThan2Seconds) {
  _pressed4LessThan2Seconds = true;
  /*
#ifdef DEBUG
  Serial << endl << F("Pressing button for less than 2sec";
#endif
*/
}
    }
  } else if (currentState == previousState && startTime > 0) {

    if (time - startTime >= configuration.bouncing && time - startTime < 1000) {
      pressed4LessThan2Seconds = true;
      /*
#ifdef DEBUG
      Serial << endl << F("Button pressed for more less than 2sec";
#endif
*/
    }

    if (time - startTime >= 1000 && time - startTime < 3000) {
      pressed42Seconds = true;
/*
#ifdef DEBUG
      Serial << endl << F("Button pressed for 2sec";
#endif
  */  }

if (time - startTime >= 3000) {
  pressed45Seconds = true;
  /*
#ifdef DEBUG
  Serial << endl << F("Button pressed for more than 5sec";
#endif
*/
}

_pressed4LessThan2Seconds = false;
_pressed42Seconds = false;
_pressed45Seconds = false;

startTime = 0;
_pressed = false;
  }
}

boolean SCREEN::isPressed() {
  if (pressed4LessThan2Seconds) {
    pressed4LessThan2Seconds = false;
    // pressed = flas
    return true;
  } else {
    return false;
  }
}

boolean SCREEN::isPressed2() {
  if (pressed42Seconds) {
    pressed42Seconds = false;
    return true;
  } else {
    return false;
  }
}

boolean SCREEN::isPressed5() {
  if (pressed45Seconds) {
    pressed45Seconds = false;
    return true;
  } else {
    return false;
  }
}
