void eventsListener() {
  /* Event handler: connection to wireless network has been established */
  if (Network.eventConnected()) {
#ifdef DEBUG
    Serial << endl
           << endl
           << F("#### CONNECTED TO THE WIFI ####") << endl
           << endl
           << F("Events listener: triggered";
#endif
  }
}
