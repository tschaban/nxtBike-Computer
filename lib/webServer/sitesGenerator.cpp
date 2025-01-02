#include "sitesGenerator.h"

SITEGENERATOR::SITEGENERATOR() {}

const String SITEGENERATOR::generateHeader(uint8_t redirect) {

  String page = "<!doctype html><html lang=\"";
  page += "\"><head><meta charset=\"utf-8\">";

  if (redirect > 0) {
    page += "<meta http-equiv=\"refresh\" content=\"";
    page += String(redirect);
    page += "; url=/\">";
  }

  page += "<title>";
  page += FIRMWARE_NAME;
  page += " [";
  page += FIRMWARE_VERSION;
  page += "]</title><style>";
  page += CSS;
  page += "</style></head><body>";

  page += "<h3 class=\"la\">";
  page += FIRMWARE_NAME;
  page += " [";
  page += FIRMWARE_VERSION;
  page += "]</h3><div id=\"c\">";

  return page;
}

const String SITEGENERATOR::generateOneColumnLayout(uint8_t redirect) {
  String page = generateHeader(redirect);
  page += "<div id=\"r\">";
  return page;
}

String SITEGENERATOR::showIndex() {
  String body = "<ul>";
  body += "<li><a href=\"/?o=";
  body += SITE_UPGRADE;
  body += "\" >Upgrade</a></li>";

  body += "<li><a href=\"/?o=";
  body += SITE_SHOW_FILES;
  body += "\" >Show Files</a></li>";

  body += "<li><a href=\"/?o=";
  body += SITE_RESTORE;
  body += "\" >Reset</a></li>";

  body += "<li><a href=\"/?o=";
  body += SITE_REBOOT;
  body += "\" >Reboot</a></li>";

  body += "</ul>";

  return body;
}

String SITEGENERATOR::showUpgrade() {
  String body = "<form method=\"post\" action=\"upgrade?o=";
  body += SITE_POST_UPGRADE;
  body += "\" enctype=\"multipart/form-data\">";
  body += "<fieldset><div class=\"cf\"><label>";
  body += L_SELECT_FIRMWARE;
  body += "</label><input class=\"bs\" name=\"update\" type=\"file\" "
          "accept=\".bin\"></div><p class=\"cm\">";
  body += L_UPGRADE_INFO;
  body += ".</p><button type=\"submit\" class=\"b be\">";
  body += L_UPGRADE;
  body += "</button></fieldset></form>";
  return addConfigurationBlock(L_FIRMWARE_UPGRADE, L_DONT_PLUG_OFF, body);
}

String SITEGENERATOR::showPostUpgrade(boolean status) {

  String body = "<fieldset><ul>";
  if (status) {
    body += "<li style=\"color:red\">";
    body += L_UPGRADE_FAILED;
  } else {
    body += "<li>";
    body += L_UPGRADE_SUCCESSFUL;
  }
  body += "</li><li>";
  body += L_DEVICE_WILL_BE_REBOOTED;
  body += "...</li></fieldset>";
  return addConfigurationBlock(L_FIRMWARE_UPGRADE, "", body);
}

String SITEGENERATOR::showRestore() {
  String body = "<form method=\"post\" action=\"index?o=";
  body += SITE_RESTORE;
  body += "\">";
  body += "<fieldset><div class=\"cf\"><label>";
  body += L_SELECT_FIRMWARE;
  body += "</label><input class=\"bs\" name=\"t\" "
          "type=\"number\"></div><button type=\"submit\" class=\"b be\">";
  body += "Save";
  body += "</button></fieldset></form>";
  return addConfigurationBlock(L_FIRMWARE_UPGRADE, L_DONT_PLUG_OFF, body);
}

String SITEGENERATOR::showFiles() {

  DATA Data;
  SpeedometerDataType speedometerData = Data.getSpeedometterData();
  char _floatToString[12];
  dtostrf(speedometerData.distance.total, 12, 6, _floatToString);
  String body = "Total distance: ";
  body += _floatToString;
  dtostrf(speedometerData.distance.current, 12, 6, _floatToString);
  body += "<br>Current distance: ";
  body += _floatToString;
  dtostrf(speedometerData.speed.max, 12, 0, _floatToString);
  body += "<br>Max speed: ";
  body += _floatToString;
  dtostrf(speedometerData.speed.avarage, 12, 0, _floatToString);
  body += "<br>Avarage: ";
  body += _floatToString;
  body += "<br>Timer: ";
  body += speedometerData.timer;
  body += "<br>Timer in ride: ";
  body += speedometerData.timeInMove;
  return body;
}

const String SITEGENERATOR::generateFooter() {
  String body = "</div></div>";
  body += "</body></html>";
  return body;
}

String SITEGENERATOR::addConfigurationBlock(const String title,
                                            const String description,
                                            const String body) {
  String page = "<div class=\"ci\">";
  page += "<h1>" + title + "</h1>";
  page += "<p class=\"cd\">" + description + "</p>";
  page += body;
  page += "</div>";

  return page;
}
