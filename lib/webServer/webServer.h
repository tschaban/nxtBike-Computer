#ifndef _Web_Server_h
#define _Web_Server_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <dataAccess.h>
#include <sitesGenerator.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

struct SITEType {
  uint8_t ID;
  boolean reboot = false;
  uint8_t rebootTime = 0;
};

class WEBSERVER {

private:
  DATA Data;

  ESP8266WebServer server;

  HTTPCOMMANDType httpCommand;
  // Once HTTP API requet is recieved it's set to true
  boolean receivedHTTPCommand = false;

  SITEGENERATOR Site;

  boolean upgradeFailed = false;

  /* Method gets url Option parameter value */
  uint8_t getSiteID();

  String generateSite(SITEType *siteConfig);

  /* Methods get POST data (for saveing) */

public:
  WEBSERVER();

  /* Method pushes HTML site from WebServer */
  void publishHTML(String page);

  /* Method initialize WebServer and Updater server */
  void begin();

  /* Method listens for HTTP requests */
  void listener();

  /* Method listens for onNotFound */
  void onNotFound(ESP8266WebServer::THandlerFunction fn);

  /* Method adds URL for listen */
  void handle(const char *uri, ESP8266WebServer::THandlerFunction handler);
  void handleFirmwareUpgrade(const char *uri,
                             ESP8266WebServer::THandlerFunction handlerUpgrade,
                             ESP8266WebServer::THandlerFunction handlerUpload);

  /* Method generate HTML side. It reads also data from HTTP requests arguments
   * and pass them to Configuration Panel class */
  void generate(boolean upload = false);

  /* Method listens for HTTP API requests. If get True command is in httpCommand
   */
  boolean httpAPIlistener();
};

#endif
