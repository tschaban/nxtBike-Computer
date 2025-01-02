#include "dataAccess.h"

DATA::DATA() {

}

SpeedometerDataType DATA::getSpeedometterData() {
  SpeedometerDataType _data;

#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Reading File -------------------");
  Serial << endl << F("File: ") << FILE_SPEEDOMETER;
#endif

  File speedometterFile = LittleFS.open(FILE_SPEEDOMETER, "r");

  if (speedometterFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = speedometterFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    speedometterFile.readBytes(buf.get(), size);
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      _data.timer = root["timer"].as<unsigned int>();
      _data.timeInMove = root["timeInMove"].as<unsigned int>();
      _data.speed.max = root["speed"]["max"].as<double>();
      _data.speed.avarage = root["speed"]["avarage"].as<double>();
      _data.distance.total = root["distance"]["total"].as<double>();
      _data.distance.current = root["distance"]["current"].as<double>();

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << 400
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (400 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    speedometterFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << FILE_SPEEDOMETER
           << F(" not opened");
  }
#endif

#ifdef DEBUG
  Serial << endl << F("--------------------------------------------------");
#endif

  return _data;
};

/* Wrties data to speedomeeter.json */
void DATA::saveSpeedometterData(SpeedometerDataType *data) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Writing to File -------------------");
  Serial << endl << F("File: ") << FILE_SPEEDOMETER;
#endif

  File speedometterFile = LittleFS.open(FILE_SPEEDOMETER, "w");

  if (speedometterFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<400> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    JsonObject &jsonSpeed = root.createNestedObject("speed");
    JsonObject &jsonDistance = root.createNestedObject("distance");

    root["timer"] = data->timer;
    root["timeInMove"] = data->timeInMove;
    jsonSpeed["max"] = data->speed.max;
    jsonSpeed["avarage"] = data->speed.avarage;
    jsonDistance["current"] = data->distance.current;
    jsonDistance["total"] = data->distance.total;

    root.printTo(speedometterFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    speedometterFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << 400 << F(", actual JSON size: ")
           << jsonBuffer.size();

    if (400 < jsonBuffer.size() + 10) {
      Serial << endl << F("WARN: Too small buffer size");
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif

#ifdef DEBUG
  Serial << endl << F("-----------------------------------------------------");
#endif
};

boolean DATA::eventSpeedometterSave() {
  if (eventSpeedometterSaveCounter == 0) {
    eventSpeedometterSaveCounter = millis();
  }

  if (millis() - eventSpeedometterSaveCounter >=
      SPEEDOMETER_DATA_SAVE_FREQUENCY * 1000) {
    eventSpeedometterSaveCounter = millis();
    return true;
  } else {
    return false;
  }
}

LCDScreenCurrentConfigurationType DATA::getCurrentScreen() {
  LCDScreenCurrentConfigurationType _data;

#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Reading File -------------------");
  Serial << endl << F("File: ") << FILE_CURRENTSCREEN;
#endif

  File currentScreenFile = LittleFS.open(FILE_CURRENTSCREEN, "r");

  if (currentScreenFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = currentScreenFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    currentScreenFile.readBytes(buf.get(), size);
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif
      _data.screenID = root["screenID"].as<unsigned int>();
      _data.themeID = root["themeID"].as<unsigned int>();

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << 200
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (200 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    currentScreenFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << FILE_CURRENTSCREEN
           << F(" not opened");
  }
#endif

#ifdef DEBUG
  Serial << endl << F("--------------------------------------------------");
#endif

  return _data;
}

void DATA::saveCurrentScreen(LCDScreenCurrentConfigurationType *data) {
#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Writing to File -------------------");
  Serial << endl << F("File: ") << FILE_CURRENTSCREEN;
#endif

  File currentScreenFile = LittleFS.open(FILE_CURRENTSCREEN, "w");

  if (currentScreenFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

    StaticJsonBuffer<400> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["screenID"] = data->screenID;
    root["themeID"] = data->themeID;

    root.printTo(currentScreenFile);
#ifdef DEBUG
    root.printTo(Serial);
#endif
    currentScreenFile.close();

#ifdef DEBUG
    Serial << endl
           << F("INFO: Data saved") << endl
           << F("INFO: JSON: Buffer size: ") << 200 << F(", actual JSON size: ")
           << jsonBuffer.size();

    if (200 < jsonBuffer.size() + 10) {
      Serial << endl << F("WARN: Too small buffer size");
    }
#endif
  }
#ifdef DEBUG
  else {
    Serial << endl << F("ERROR: failed to open file for writing");
  }
#endif

#ifdef DEBUG
  Serial << endl << F("-----------------------------------------------------");
#endif
}

void DATA::formatFileSystem() {
#ifdef DEBUG
  Serial << endl << F("---------------- RESETTING DEVICE -----------------");
  Serial << endl << F("Formating: ");
#endif
  LittleFS.format();
  delay(1000);
#ifdef DEBUG
  Serial << F("DONE");
  Serial << endl << F("Creating speedometer file");
#endif
  createSpeedometterFile();
  delay(1000);
#ifdef DEBUG
  Serial << endl << F("Creating screen configuration file");
#endif
  createCurrentScreenFile();
#ifdef DEBUG
  Serial << endl << F("Completed");
  Serial << endl << F("---------------------------------------------------");
#endif
}

void DATA::createSpeedometterFile() {
  SpeedometerDataType _data;
  _data.distance.current = 0;
  _data.distance.total = SPEEDOMETER_DEFAULT_TOTAL_DISTANCE;
  _data.speed.max = 0;
  _data.speed.avarage = 0;
  _data.timer = 0;
  _data.timeInMove = 0;
  saveSpeedometterData(&_data);
}

void DATA::createCurrentScreenFile() {
  LCDScreenCurrentConfigurationType data;
  data.screenID = SCREEN_SPEEDOMETER;
  data.themeID = THEME_NORMAL;
  saveCurrentScreen(&data);
}

void DATA::createdBackup() {
  SpeedometerDataType _data;

  _data = getSpeedometterData();

#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Archiving File -------------------");
#endif

  if (_data.distance.total > SPEEDOMETER_DEFAULT_TOTAL_DISTANCE) {

#ifdef DEBUG
    Serial << endl
           << endl
           << F("----------------- Writing to File -------------------");
    Serial << endl << F("File: ") << FILE_SPEEDOMETER_BACKUP;
#endif

    File speedometterFile = LittleFS.open(FILE_SPEEDOMETER_BACKUP, "w");

    if (speedometterFile) {
#ifdef DEBUG
      Serial << F("success") << endl << F("INFO: Writing JSON: ");
#endif

      StaticJsonBuffer<400> jsonBuffer;
      JsonObject &root = jsonBuffer.createObject();
      JsonObject &jsonSpeed = root.createNestedObject("speed");
      JsonObject &jsonDistance = root.createNestedObject("distance");

      root["timer"] = _data.timer;
      root["timeInMove"] = _data.timeInMove;
      jsonSpeed["max"] = _data.speed.max;
      jsonSpeed["avarage"] = _data.speed.avarage;
      jsonDistance["current"] = _data.distance.current;
      jsonDistance["total"] = _data.distance.total;

      root.printTo(speedometterFile);
#ifdef DEBUG
      root.printTo(Serial);
#endif
      speedometterFile.close();

#ifdef DEBUG
      Serial << endl
             << F("INFO: Data saved") << endl
             << F("INFO: JSON: Buffer size: ") << 400
             << F(", actual JSON size: ") << jsonBuffer.size();

      if (400 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << endl << F("ERROR: failed to open file for writing");
    }
#endif

#ifdef DEBUG
    Serial << endl
           << F("-----------------------------------------------------");
#endif
  }
}

void DATA::restoreBackup() {
  SpeedometerDataType _data;

#ifdef DEBUG
  Serial << endl
         << endl
         << F("----------------- Reading File -------------------");
  Serial << endl << F("File: ") << FILE_SPEEDOMETER_BACKUP;
#endif

  File speedometterFile = LittleFS.open(FILE_SPEEDOMETER_BACKUP, "r");

  if (speedometterFile) {
#ifdef DEBUG
    Serial << F("success") << endl << F("INFO: JSON: ");
#endif

    size_t size = speedometterFile.size();
    std::unique_ptr<char[]> buf(new char[size]);
    speedometterFile.readBytes(buf.get(), size);
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(buf.get());
    if (root.success()) {
#ifdef DEBUG
      root.printTo(Serial);
#endif

      _data.timer = root["timer"].as<unsigned int>();
      _data.timeInMove = root["timeInMove"].as<unsigned int>();
      _data.speed.max = root["speed"]["max"].as<double>();
      _data.speed.avarage = root["speed"]["avarage"].as<double>();
      _data.distance.total = root["distance"]["total"].as<double>();
      _data.distance.current = root["distance"]["current"].as<double>();

      if (_data.distance.total > SPEEDOMETER_DEFAULT_TOTAL_DISTANCE) {
        saveSpeedometterData(&_data);
      }

#ifdef DEBUG
      Serial << endl
             << F("INFO: JSON: Buffer size: ") << 400
             << F(", actual JSON size: ") << jsonBuffer.size();
      if (400 < jsonBuffer.size() + 10) {
        Serial << endl << F("WARN: Too small buffer size");
      }
#endif
    }
#ifdef DEBUG
    else {
      Serial << F("ERROR: JSON not pharsed");
    }
#endif

    speedometterFile.close();
  }

#ifdef DEBUG
  else {
    Serial << endl
           << F("ERROR: Configuration file: ") << FILE_SPEEDOMETER_BACKUP
           << F(" not opened");
  }
#endif

#ifdef DEBUG
  Serial << endl << F("--------------------------------------------------");
#endif
}
