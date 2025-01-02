

void initRelay() {
  for (uint8_t i = 0; i < sizeof(Device.isRelay); i++) {
    if (Device.isRelay[i]) {
      Relay[i].begin(i);
    } else {
      break;
    }
  }
}

void mainRelay() {
  for (uint8_t i = 0; i < sizeof(Device.isRelay); i++) {
  }
}
