#ifndef _Web_Server_h
#define _Web_Server_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <WebServer.h>
#include <Update.h>
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

class HTTPSERVER {

private:
  DATA Data;

  WebServer server;

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
  HTTPSERVER();

  /* Method pushes HTML site from HTTPSERVER */
  void publishHTML(String page);

  /* Method initialize HTTPSERVER and Updater server */
  void begin();

  /* Method listens for HTTP requests */
  void listener();


  void onNotFound(WebServer::THandlerFunction fn);
  void handle(const char *uri, WebServer::THandlerFunction handler);
  void handleFirmwareUpgrade(const char *uri,
                             WebServer::THandlerFunction handlerUpgrade,
                             WebServer::THandlerFunction handlerUpload);

  /* Method generate HTML side. It reads also data from HTTP requests arguments
   * and pass them to Configuration Panel class */
  void generate(boolean upload = false);

  /* Method listens for HTTP API requests. If get True command is in httpCommand
   */
  boolean httpAPIlistener();
};

#endif
