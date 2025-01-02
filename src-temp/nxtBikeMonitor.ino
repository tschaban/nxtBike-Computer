/*

*/

/* Includes libraries for debugging in development compilation only */

#ifdef DEBUG
#include <Streaming.h>
#endif

#include <AFE-Data-Access.h>
//#include <AFE-I2C-Scanner.h>
#include <AFE-LED.h>
#include <AFE-Relay.h>
#include <AFE-Sensor-DS18B20.h>
#include <AFE-Sensor-NTC10K.h>
#include <AFE-Switch.h>
#include <AFE-Web-Server.h>
#include <AFE-WiFi.h>
#include <FS.h>

AFEDataAccess Data;
AFEWiFi Network;
AFEWebServer WebServer;
AFESwitch Switch[DEVICE_NO_OF_SWITCHES];
AFERelay Relay[DEVICE_NO_OF_RELAYS];
AFELED Led;
AFESensorDS18B20 SensorDS18B20[DEVICE_NO_OF_DS18B20];
AFESensorNTC10K SensorNTC10K;
// AFEI2CScanner I2CScanner;

float temperature;

uint8_t DeviceMode;
DEVICE Device;

void setup() {

  Serial.begin(115200);
  delay(10);

/* Turn off publishing information to Serial for production compilation */
#if !defined(DEBUG)
  Serial.swap();
#endif

#ifdef DEBUG
  Serial << endl
         << endl
         << F("################################ BOOTING "
            "################################";
#endif

  if (SPIFFS.begin()) {
#ifdef DEBUG
    Serial << endl << F("File system mounted";
  } else {
    Serial << endl << F("Failed to mount file system";
#endif
  }

#ifdef DEBUG
  Serial << endl << F("Initializing device";
#endif

  DeviceMode = Data.getDeviceMode();

#ifdef DEBUG
  Serial << endl << F("All classes and global variables initialized";
#endif

  /* Checking if the device is launched for a first time. If so it loades
   * default configuration to EEPROM */
  if (Data.isFirstTimeLaunch()) {
#ifdef DEBUG
    Serial << endl << F("First time launch";
#endif
    AFEDefaults Defaults;
    Defaults.set();
  }

  Device = Data.getDeviceConfiguration();

  /* Initializing relay */
  initRelay();
#ifdef DEBUG
  Serial << endl << F("Relay(s) initialized";
#endif

  /* Initialzing network */
  Network.begin(DeviceMode);
#ifdef DEBUG
  Serial << endl << F("Network initialized";
#endif

  /* Initializing LED, checking if LED exists is made on Class level  */
  uint8_t systeLedID = Data.getSystemLedID();
  if (systeLedID > 0) {
    Led.begin(systeLedID - 1);
  }

#ifdef DEBUG
  Serial << endl << F("System LED initialized";
#endif

  /* If device in configuration mode then it starts LED blinking */
  if (DeviceMode == MODE_ACCESS_POINT) {
    Led.blinkingOn(100);
  }

  Network.listener();

#ifdef DEBUG
  Serial << endl << F("Network listener initialized";
#endif

  /* Initializing HTTP WebServer */
  WebServer.handle("/", handleHTTPRequests);
  WebServer.handle("/favicon.ico", handleFavicon);
  WebServer.begin();
#ifdef DEBUG
  Serial << endl << F("WebServer initialized";
#endif

  /* Initializing switches */
  initSwitch();
#ifdef DEBUG
  Serial << endl << F("Switch(es) initialized";
#endif

  if (DeviceMode == MODE_NORMAL) {
    initSensorNTC10K();

#ifdef DEBUG
    Serial << endl << F("NTC10K initialized";
#endif

    initSensorDS18B20();
  }

#if defined(DEBUG)
  /* Scanning I2C for devices */
  if (DeviceMode == MODE_NORMAL) {
    // I2CScanner.scanAll();
  }

  Serial << endl
         << F("########################### BOOTING COMPLETED "
            "###########################"
         << endl;
#endif
}

void loop() {
  if (DeviceMode != MODE_ACCESS_POINT) {
    if (Network.connected()) {
      if (DeviceMode == MODE_NORMAL) {

        /* Triggerd when connectes/reconnects to WiFi */
        eventsListener();

        /* Listens for HTTP requsts. Both for configuration panel HTTP
         * requests or HTTP API requests if it's turned on */
        WebServer.listener();

        /* Relay related events */
        mainRelay();
        mainSensorDS18B20();
        mainSensorNTC10K();

      } else { /* Device runs in configuration mode over WiFi */
        if (!Led.isBlinking()) {
          Led.blinkingOn(100);
        }
        WebServer.listener();
      }
    }

    else {
      if (DeviceMode == MODE_CONFIGURATION && Led.isBlinking()) {
        Led.blinkingOff();
      }
    }
    Network.listener();
  } else { /* Deviced runs in Access Point mode */
    Network.APListener();
    WebServer.listener();
  }

  /* Listens for switch events */
  mainSwitchListener();
  mainSwitch();

  /* Led listener */
  Led.loop();

/* Debug information */
#if defined(DEBUG)
  if (DeviceMode == MODE_NORMAL) {
    debugListener();
  }
#endif
}
