/* Initializing sensor */
void initSensorDS18B20() {
  if (Device.isDS18B20[0]) {
    SensorDS18B20[0].begin();
  }
}

/* Main code for processing sesnor */
void mainSensorDS18B20() {
  if (Device.isDS18B20[0]) {
    /* Sensor: listener */
    SensorDS18B20[0].listener();
    if (SensorDS18B20[0].isReady()) {
      Led.on();
      temperature = SensorDS18B20[0].getTemperature();
      Led.off();
    }
  }
}
