/* AFE Firmware for smart home devices
  LICENSE: https://github.com/tschaban/AFE-Firmware/blob/master/LICENSE
  DOC: https://www.smartnydom.pl/afe-firmware-pl/ */

#ifndef _AFE_Sites_Generator_h
#define _AFE_Sites_Generator_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <CSS.h>
#include <WiFi.h>
#include <configuration.h>
#include <dataAccess.h>
#include <en_EN.h>

#ifdef DEBUG
#include <Streaming.h>
#endif

class SITEGENERATOR {

private:
  const String generateHeader(uint8_t redirect);

  /* Method addes configuration block to the site */
  String addConfigurationBlock(const String title, const String description,
                               const String body);

public:
  /* Constructor*/
  SITEGENERATOR();

  /* Method generates site header with menu. When redirect param is diff than 0
    then it will redirect page to main page after redirect param time (in sec)
   */
  const String generateOneColumnLayout(uint8_t redirect = 0);

  /* Method generates site footer */
  const String generateFooter();

  /* These methods generates firmware upgrade sections */
  String showIndex();
  String showUpgrade();
  String showPostUpgrade(boolean status);
  String showRestore();
  String showFiles();
};

#endif
