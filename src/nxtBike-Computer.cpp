#include "nxtBike-Computer.h"
/*
void speedometerEnabled(boolean enabled)
{
  if (enabled)
  {
    attachInterrupt(digitalPinToInterrupt(SpeedoMeter.configuration.GPIO),
                    newImpulse, RISING);
  }
  else
  {
    detachInterrupt(digitalPinToInterrupt(
        digitalPinToInterrupt(SpeedoMeter.configuration.GPIO)));
  }
}
*/
TaskHandle_t Core0Code;
TaskHandle_t Core1Code;

void mainOnCore0(void *pvParameters)
{
#ifdef DEBUG
  Msg->printInformation("Core 0 is running", F("BOOT"));
#endif
  for (;;)
  {
   HttpServer->listener();
  }
}

void mainOnCore1(void *pvParameters)
{
#ifdef DEBUG
  Msg->printInformation("Core 1 is running", F("BOOT"));
#endif
  for (;;)
  {
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10);

#ifdef DEBUG
  Msg->printHeader();
  Msg->printInformation("Starting...", F("BOOT"));
  Msg->getESPHardwareInformation();
  Msg->getFirmwareFlashInformation();
  Msg->getFreeMemorySize();
  Msg->printBulletPoint(F("Mounting file system: "));
#endif

  boolean success = LITTLEFS.begin(true);

#ifdef DEBUG
  if (success)
  {
    Msg->printValue(F("OK"));
   /*
    Msg->printBulletPoint(F("Formatting Flash: "));
    success = LITTLEFS.format();
    if (success) {
      Msg->printValue(F("OK"));
    } else {
      Msg->printValue(F("FAILURE"));
    }
    */
    Msg->getFileSystemDubugInformation();
  }
  else
  {
    Msg->printValue(F("FAILURE"));
  }
#endif

#ifdef DEBUG
  Msg->printBulletPoint(F("Initializing Led"));
#endif

  Led.on();

#ifdef DEBUG
  Msg->printBulletPoint(F("Initializing LCD"));
#endif

  //uint8_t lastScreen = Screen.getActiveScreenID();
  //nexInit();
  //Screen.set(SCREEN_SPLASH);

#ifdef DEBUG
  Msg->printBulletPoint(F("Initializing WiFi"));
#endif
  WiFiNetwork->begin();

  /*
#ifdef DEBUG
  Msg->printBulletPoint(F("Initializing Speedometer"));
#endif
  pinMode(SpeedoMeter.configuration.GPIO, INPUT_PULLUP);
  speedometerEnabled(true);

  SpeedoMeter.data = Data.getSpeedometterData();
  Screen.refreshScreenS2(&SpeedoMeter.data);

  lastSavedTotalSpeed = SpeedoMeter.data.distance.total;
*/
#ifdef DEBUG
  Msg->printBulletPoint(F("Initializing HttpServer"));
#endif

  HttpServer->handle("/", handleHTTPRequests);
  HttpServer->handle("/favicon.ico", handleFavicon);
  HttpServer->handle("/debug", handleDebug);
  HttpServer->handleFirmwareUpgrade("/upgrade", handleHTTPRequests, handleUpload);
  HttpServer->begin();
/*
#ifdef DEBUG
  Msg->printBulletPoint(F("Sync LCD"));
#endif
  Screen.set(lastScreen);
  Screen.showSyncStatus(SYNC_COMPLETED);

  Led.off();

#ifdef DEBUG
  Msg->printBulletPoint(F("Initializing Cores"));
#endif
*/
  // create a task that executes the Task0code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(mainOnCore0, "Code run on Core 0", 10000, NULL, 1, &Core0Code, 0);
  // create a task that executes the Task0code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(mainOnCore1, "Code run on Core 1", 10000, NULL, 1, &Core0Code, 1);

#ifdef DEBUG
  Msg->printBulletPoint(F("Boot completed"));
  Msg->printHeader();
#endif
}

void loop() {}
