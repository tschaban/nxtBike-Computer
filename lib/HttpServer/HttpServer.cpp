/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#include "HttpServer.h"

HTTPSERVER::HTTPSERVER() {}

void HTTPSERVER::begin() { server.begin(); }

String HTTPSERVER::generateSite(SITEType *siteConfig) {
  String page;
  page = Site.generateOneColumnLayout(siteConfig->rebootTime);

  switch (siteConfig->ID)
  {
  case SITE_INDEX:
    page += Site.showIndex();
    break;
  case SITE_UPGRADE:
    page += Site.showUpgrade();
    break;
  case SITE_POST_UPGRADE:
    page += Site.showPostUpgrade(upgradeFailed);
    break;
  case SITE_RESTORE:
    Data.formatFileSystem();
    page += Site.showIndex();
    break;
  case SITE_REBOOT:
    page += Site.showIndex();
    break;
  case SITE_SHOW_FILES:
    page += Site.showFiles();
    break;
  }

  page += Site.generateFooter();

  return page;
}

void HTTPSERVER::generate(boolean upload) {

  SITEType siteConfig;

  siteConfig.ID = getSiteID();

  switch (siteConfig.ID)
  {
  case SITE_RESTORE:
    siteConfig.rebootTime = 8;
    siteConfig.reboot = true;
    break;
  case SITE_REBOOT:
    siteConfig.rebootTime = 1;
    siteConfig.reboot = true;
  case SITE_UPGRADE:
    break;
  case SITE_POST_UPGRADE:
    if (!upload)
    {
      siteConfig.rebootTime = 8;
      siteConfig.reboot = true;
    }
    break;
  }

  if (upload) {
    HTTPUpload &upload = server.upload();
    String _updaterError;
    boolean _success = false;
    if (upload.status == UPLOAD_FILE_START)  {

#ifdef DEBUG
      Serial << endl
             << endl
             << F("---------------- Firmware upgrade -----------------");
      Serial << endl
             << F("Update: ") << upload.filename.c_str();
#endif

      uint32_t maxSketchSpace = UPDATE_SIZE_UNKNOWN;

      if (!Update.begin(maxSketchSpace)) {
#ifdef DEBUG
        Update.printError(Serial);
#endif
      }
    } else if (upload.status == UPLOAD_FILE_WRITE && !_updaterError.length())   {

#ifdef DEBUG
      // Serial << endl << (upload.totalSize / 1024) << F(" kB");
      Serial << F(">");
#endif

      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
#ifdef DEBUG
        Update.printError(Serial);
#endif
      }
    } else if (upload.status == UPLOAD_FILE_END && !_updaterError.length())  {
      if (Update.end(true)) { 
#ifdef DEBUG
        Serial << upload.totalSize;
#endif
      }
#ifdef DEBUG
      else {
        Update.printError(Serial);
      }
#endif
    } else if (upload.status == UPLOAD_FILE_ABORTED) {
      Update.end();
#ifdef DEBUG
      Serial << endl
             << "Warning: Update was aborted";
#endif
    } 
  } else {

#ifdef DEBUG
  Serial << endl
         << endl
         << F("---------------- Opening WebSite -----------------");
  Serial << endl
         << F("Site ID: ") << siteConfig.ID;
  Serial << endl
         << F("Reboot: ") << (siteConfig.reboot ? "Yes" : "No");
  if (siteConfig.reboot)
  {
    Serial << endl
           << F(" - Time: ") << siteConfig.rebootTime;
  }

  Serial << endl
         << F("--------------------------------------------------") << endl;

#endif
  publishHTML(generateSite(&siteConfig));
  }

  /* Rebooting device */
  if (siteConfig.reboot) {
#ifdef DEBUG
  Serial << endl
         << F("Erasing config");
#endif
     delay(1000);
#ifdef DEBUG
  Serial << endl
         << F("Rebooting");
#endif
     ESP.restart();
  }
}

/* Methods related to the url request */

uint8_t HTTPSERVER::getSiteID()
{

  if (server.hasArg("o"))
  {
    return server.arg("o").toInt();
  }
  else
  {
    return SITE_INDEX;
  }
}

/* Server methods */

void HTTPSERVER::listener() { server.handleClient(); }

boolean HTTPSERVER::httpAPIlistener() { return receivedHTTPCommand; }

void HTTPSERVER::publishHTML(String page)
{
  uint16_t pageSize = page.length();
  uint16_t size = 1024;
  server.setContentLength(pageSize);
  if (pageSize > size)
  {
    server.send(200, "text/html", page.substring(0, size));
    uint16_t transfered = size;
    uint16_t nextChunk;
    while (transfered < pageSize)
    {
      nextChunk = transfered + size < pageSize ? transfered + size : pageSize;
      server.sendContent(page.substring(transfered, nextChunk));
      transfered = nextChunk;
    }
  }
  else
  {
    server.send(200, "text/html", page);
  }
}

void HTTPSERVER::handle(const char *uri,
                           WebServer::THandlerFunction handler)
{
  server.on(uri, handler);
}

void HTTPSERVER::handleFirmwareUpgrade(
    const char *uri, WebServer::THandlerFunction handlerUpgrade,
    WebServer::THandlerFunction handlerUpload)
{
  server.on(uri, HTTP_POST, handlerUpgrade, handlerUpload);
}

void HTTPSERVER::onNotFound(WebServer::THandlerFunction fn)
{
  server.onNotFound(fn);
}
