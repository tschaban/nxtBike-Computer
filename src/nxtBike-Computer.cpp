#include "nxtBike-Computer.h"

TaskHandle_t Core0Code;
//TaskHandle_t Core1Code;

void mainOnCore0(void *pvParameters)
{
#ifdef DEBUG
  Firmware->System->Msg->printInformation("Core 0 is running", F("BOOT"));
#endif
  for (;;)
  { 
    Firmware->API->Network->listener();
    // HttpServer->listener();
  }
}

void mainOnCore1(void *pvParameters)
{
#ifdef DEBUG
  Firmware->System->Msg->printInformation("Core 1 is running", F("BOOT"));
#endif
  for (;;)
  {
  }
}

void setup()
{
  Serial.begin(115200);
  delay(10);

  // Fix for Watchdog Timer
  rtc_wdt_protect_off();    // Turns off the automatic wdt service
  rtc_wdt_enable();         // Turn it on manually
  rtc_wdt_set_time(RTC_WDT_STAGE0, 20000);  // Define how long you desire to let dog wait.

#ifdef DEBUG
  Firmware->System->Msg->printHeader();
  Firmware->System->Msg->printInformation("Starting...", F("BOOT"));
  Firmware->System->Msg->getFreeMemorySize();
  Firmware->System->Msg->getESPHardwareInformation();
  Firmware->System->Msg->getFirmwareFlashInformation();

  Firmware->System->Msg->printBulletPoint(F("Mounting file system: "));
#endif

  Firmware->init();
  
  #ifdef DEBUG
  Firmware->System->Msg->printInformation(F("Boot completed"),F("BOOT"));
  Firmware->System->Msg->printHeader();
#endif

  // create a task that executes the Task0code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(mainOnCore0, "Code run on Core 0", 10000, NULL, 1, &Core0Code, 0);
  // create a task that executes the Task0code() function, with priority 1 and executed on core 1
  //xTaskCreatePinnedToCore(mainOnCore1, "Code run on Core 1", 10000, NULL, 1, &Core0Code, 1);


}

void loop() {}
