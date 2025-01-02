/* Initializing Switches */
void initSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.isSwitch); i++) {
    if (Device.isSwitch[i]) {
      Switch[i].begin(i);
    } else {
      break;
    }
  }
}

/* Method processes Switch related events */
void mainSwitch() {
  for (uint8_t i = 0; i < sizeof(Device.isSwitch); i++) {
    if (Device.isSwitch[i]) {
      /* One of the switches has been shortly pressed */
      if (Switch[i].isPressed()) {

        Led.on();
        Led.off();
      }
    } else {
      break;
    }
  }
}

void mainSwitchListener() {

  /* Listens for switch events */
  for (uint8_t i = 0; i < sizeof(Device.isSwitch); i++) {
    if (Device.isSwitch[i]) {

      Switch[i].listener();

      /* One of the Multifunction switches pressed for 10 seconds */
      if (Switch[i].getFunctionality() == SWITCH_MULTI) {
        AFEDevice deviceControl;
        if (Switch[i].is10s()) {

          DeviceMode == MODE_NORMAL ? deviceControl.reboot(MODE_ACCESS_POINT)
                                    : deviceControl.reboot(MODE_NORMAL);
        } else if (Switch[i].is5s()) {
          DeviceMode == MODE_NORMAL ? deviceControl.reboot(MODE_CONFIGURATION)
                                    : deviceControl.reboot(MODE_NORMAL);
        } else if (Switch[i].is30s()) {
          Led.on();
          AFEDefaults Defaults;
          Defaults.set();
          Led.off();
          deviceControl.reboot(MODE_ACCESS_POINT);
        }
      }
    } else {
      break;
    }
  }
}
