#if defined(DEBUG)

unsigned long debugStartTime = 0;

void debugListener() {
  unsigned long time = millis();

  if (debugStartTime == 0) { // starting timer. used for switch sensitiveness
    debugStartTime = time;
  }

  if (time - debugStartTime >= DEBUG_INTERVAL * 1000) {
    Serial << endl << endl << F("----------- DEBUGGER ------------";
    getAvailableMem();
    getFileSystemInfo();
    debugStartTime = 0;
    Serial << endl << F("---------------------------------") << endl;
  }
}

void getAvailableMem() {

  Serial << endl
         << F("Free Memory = ") << system_get_free_heap_size() / 1024 << F("kB";
}

void getFileSystemInfo() {
  FSInfo fileSystem;
  SPIFFS.info(fileSystem);
  Serial << endl
         << F("File system. Used: ") << fileSystem.usedBytes / 1024 << F("kB from "
         << fileSystem.totalBytes / 1024 << F("kB";
}

#endif
