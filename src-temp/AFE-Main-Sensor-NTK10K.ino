/* Initializing sensor */
void initSensorNTC10K() {
  if (Device.isNTC10K) {
    SensorNTC10K.begin();
  }
}

/* Main code for processing sesnor */
void mainSensorNTC10K() {
  if (Device.isNTC10K) {
    /* Sensor: listener */
    SensorNTC10K.listener();
    if (SensorNTC10K.isReady()) {
      float temperature;
      Led.on();
      temperature = SensorNTC10K.getTemperature();
      Led.off();
    }
  }
}
