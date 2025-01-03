#include "nxtBike-Computer.h"

TaskHandle_t Task0;
TaskHandle_t Task1;



void Task0code(void* pvParameters) {
  Serial << "Task0 running on core ";
  Serial << xPortGetCoreID();

  for (;;) {
    Serial << "hey, this is core : ";
     Serial << xPortGetCoreID() << endl;
    delay((int)random(1000, 2000));
  }
}


void Task1code(void* pvParameters) {
  Serial << "Task1 running on core ";
  Serial << xPortGetCoreID();

  for (;;) {
    // do something interesting
    Serial << "hey, this is core : ";
     Serial << xPortGetCoreID() << endl;
     delay((int)random(1000, 2000));
  }
}


void setup() {
  Serial.begin(115200);
  delay(10);

  //create a task that executes the Task0code() function, with priority 1 and executed on core 0
  xTaskCreatePinnedToCore(Task0code, "Task0", 10000, NULL, 1, &Task0, 0);
  //create a task that executes the Task0code() function, with priority 1 and executed on core 1
  xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 1);
}


void loop(){}
