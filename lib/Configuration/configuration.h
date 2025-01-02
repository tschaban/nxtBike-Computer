#ifndef _configuration_h
#define _configuration_h

#define FIRMWARE_NAME "NxtBike Firmware"
#define FIRMWARE_VERSION "1.0 BETA 10"

#define WIFI_WAIT_TIME 30 // 30 seconds
#define WIFI_ACCESS_POINT 0
#define WIFI_CLIENT 1
#define WIFI_MODE WIFI_ACCESS_POINT

#define GPIO_SPEEDOMETER 4
#define GPIO_SCREEN_CHANGE 14
#define GPIO_LED 2
#define GPIO_NEXTION_RX 13 // lib-external\Nextion\NexHardware.cpp
#define GPIO_NEXTION_TX 12 // lib-external\Nextion\NexHardware.cpp

#define SPEEDOMETER_DEFAULT_TOTAL_DISTANCE 15488
#define SPEEDOMETER_MAX_SPEED 55
#define SPEEDOMETER_MAX_SPEED_REFRESH_TIME 1000
#define SPEEDOMETER_BATTERY_LEVEL_REFRESH_TIME 5 // in sec
#define SPEEDOMETER_TEMPERATURE_REFRESH_TIME 10  // in sec
#define SPEEDOMETER_NUMBER_OF_IMPULSES_EXPTECTED 5

#define SPEEDOMETER_BACKUP_RESTORE_TIME 600 // in sec

#define SPEEDOMETER_DATA_SAVE_FREQUENCY 10 // in sec
#define SPEEDOMETER_DATA_SAVE_SPEED 40     // in sec

#define VOLTAGE_MAX_ACC_VOLTAGE 1        // 0.97034 // at ACC
#define VOLTAGE_MAX_MEASURE_VOLTAGE 83.9 // max measure voltage
#define VOLTAGE_MIN_MEASURE_VOLTAGE 62   // min measure voltage
#define VOLTAGE_CORRECTION -2.2          // min measure voltage

#define SCREEN_SPLASH 0
#define SCREEN_SPEEDOMETER 1
#define SCREEN_SPEED 2
#define SCREEN_BUTTON_BOUNCING 50

#define SYNC_COMPLETED 0
#define SYNC_NEW_DATA 1
#define SYNC_WAITING_TO_SYNC 2

#define THEME_NORMAL 1
#define THEME_LIGHT 2
#define THEME_DARK 3

#define LED_CHANGE_TO_OPPOSITE 0

#define NETWORK_SSID FIRMWARE_NAME

/* Config sites IDs */
#define SITE_INDEX 1
#define SITE_UPGRADE 2
#define SITE_POST_UPGRADE 3
#define SITE_RESTORE 4
#define SITE_REBOOT 5
#define SITE_SHOW_FILES 6

#define FILE_SPEEDOMETER "/speedometter.json"
#define FILE_CURRENTSCREEN "/current-screen.json"

#define FILE_SPEEDOMETER_BACKUP "/speedometter.archive.json"

#define FILE_NOT_OPENED 0
#define FILE_OPEN_FOR_READ 1
#define FILE_OPEN_FOR_WRITE 2

/* Data Types */

struct HTTPCOMMANDType {
  char device[16];
  char name[16];
  char command[32];
  char source[9];
};

struct LCDScreenConfigurationType {
  uint8_t buttonGPIO = GPIO_SCREEN_CHANGE;
  uint8_t bouncing = SCREEN_BUTTON_BOUNCING;
};

struct LCDScreenCurrentConfigurationType {
  uint8_t screenID = SCREEN_SPEEDOMETER;
  uint8_t themeID = THEME_NORMAL;
};

struct SpeedometerConfigurationType {
  byte GPIO = GPIO_SPEEDOMETER;
  uint8_t signalBouncing = 10;
  uint16_t refreshFrequency = SPEEDOMETER_MAX_SPEED_REFRESH_TIME;
  uint16_t refreshBatterFrequency = SPEEDOMETER_BATTERY_LEVEL_REFRESH_TIME;
  uint16_t refreshTemperatureFrequency = SPEEDOMETER_TEMPERATURE_REFRESH_TIME;
  double wheelSize = 217; // 69 cm
};

struct SpeedometerSpeedType {
  double current = 0;
  double avarage = 0;
  double max = 0;
  uint8_t maxPercent = 0;
};

struct SpeedometerDistanceType {
  double current = 0;
  double total = 0;
};

struct SpeedometerDataType {
  SpeedometerSpeedType speed;
  SpeedometerDistanceType distance;
  uint16_t timer = 0;
  uint16_t timeInMove = 0;
};
#endif
