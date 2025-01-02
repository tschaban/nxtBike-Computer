#include "voltage.h"

VOLTAGE::VOLTAGE() {}

VoltageDataType VOLTAGE::get() {
  ready = false;
  return data;
}

void VOLTAGE::measure(uint32_t measureInterval) {
  uint32_t time = millis();

  if (startTime == 0) {
    startTime = time;
  }

  if (time - startTime >= measureInterval) {

    if (counterOfSamplings < configuration.numberOfSamples) {
      temporaryAnalogData += analogRead(configuration.GPIO);
      counterOfSamplings++;
    } else {

      analogData =
          (uint16_t)(temporaryAnalogData / configuration.numberOfSamples);

      double _inputVoltage = (double)(configuration.maxVCC * analogData / 1024);

      data.voltage = ((_inputVoltage *
                       (configuration.divider.Ra + configuration.divider.Rb)) /
                      configuration.divider.Rb) +
                     VOLTAGE_CORRECTION;

      if (data.voltage - configuration.minVCCMeasure > 0) {
        data.percent =
            round(((data.voltage - configuration.minVCCMeasure) * 100) /
                  (configuration.maxVCCMeasure - configuration.minVCCMeasure));
      } else {
        data.percent = 0;
      }

      //  data.voltage = analogData;
      /*
#ifdef DEBUG
      Serial << endl
             << F(" - Number of samples: ") << counterOfSamplings << endl
             << F(" - Analog value = ") << analogData << endl
             << F(" - Input voltage = ") << _inputVoltage << endl
             << F(" - Voltage = ") << data.voltage << endl
             << F(" - percent = ") << data.percent << endl
             << F(" - Sampling time = ") << millis() - startTime - measureInterval
             << F("msec.";
#endif
*/
      counterOfSamplings = 0;
      temporaryAnalogData = 0;
      ready = true;
      startTime = 0;
    }
  }
}
