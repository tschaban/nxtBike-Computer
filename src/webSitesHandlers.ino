
#ifndef _Web_Handlers_h
#define _Web_Handlers_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

/* Method handles favicon.ico request */
void handleFavicon() {}

/* Method handles all HTTP request */
void handleHTTPRequests() { WebServer.generate(); }
void handleUpload() { WebServer.generate(true); }

void handleOnNotFound() {
  String page = "<head><meta http-equiv=\"refresh\" content=\"0; "
                "url=http://192.168.5.1/\" /></head><body><p>Opening "
                "configuration site ...</p></body>";
  WebServer.publishHTML(page);
}

void handleDebug() {
  String page = "";

  page += Voltage.data.percent;
  page += "<br>";
  page += Voltage.data.voltage;

  WebServer.publishHTML(page);
}
#endif
