/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "LED.h"

LED::LED() {
  pinMode(LEDConfiguration.gpio, OUTPUT);
  LEDConfiguration.changeToOppositeValue
      ? digitalWrite(LEDConfiguration.gpio, LOW)
      : digitalWrite(LEDConfiguration.gpio, HIGH);
}

void LED::on() {
  if (LEDConfiguration.changeToOppositeValue) {
    set(HIGH);
  } else {
    set(LOW);
  }
}

void LED::off() {
  if (LEDConfiguration.changeToOppositeValue) {
    set(LOW);
  } else {
    set(HIGH);
  }
}

void LED::blink(unsigned int duration) {
  on();
  delay(duration);
  off();
}

void LED::blinkingOn(unsigned long blinking_interval) {
  interval = blinking_interval;
  blinking = true;
}

void LED::blinkingOff() { blinking = false; }

boolean LED::isBlinking() { return blinking; }

void LED::loop() {
  if (blinking) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      toggle();
    }
  }
}

void LED::set(uint8_t state) {

  if (digitalRead(LEDConfiguration.gpio) != state) {
    digitalWrite(LEDConfiguration.gpio, state);
  }
}

void LED::toggle() {

  digitalRead(LEDConfiguration.gpio) == HIGH
      ? digitalWrite(LEDConfiguration.gpio, LOW)
      : digitalWrite(LEDConfiguration.gpio, HIGH);
}
