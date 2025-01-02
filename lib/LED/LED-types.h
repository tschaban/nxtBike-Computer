#ifndef _LED_types_h
#define _LED_types_h

#include <configuration.h>

struct LEDType {
  uint8_t gpio = GPIO_LED;
  boolean changeToOppositeValue = LED_CHANGE_TO_OPPOSITE;
};

#endif
