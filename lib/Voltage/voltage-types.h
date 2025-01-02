#ifndef _voltage_types_h
#define _voltage_types_h

#include <configuration.h>

struct VoltageDividerType {
  double Ra = 68450;
  double Rb = 680.5;
};

struct VoltageConfigurationType {
  uint8_t GPIO = 17;
  uint32_t interval = 60000;
  uint16_t numberOfSamples = 5;
  double maxVCC = VOLTAGE_MAX_ACC_VOLTAGE;
  VoltageDividerType divider;
  double minVCCMeasure = VOLTAGE_MIN_MEASURE_VOLTAGE;
  double maxVCCMeasure = VOLTAGE_MAX_MEASURE_VOLTAGE;
};

struct VoltageDataType {
  uint8_t percent;
  double voltage;
};

#endif
