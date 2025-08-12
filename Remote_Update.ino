#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <IRsend.h>
#include <NTPClient.h> 
#include <WiFiUdp.h>
#include <TimeLib.h> 
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <EEPROM.h> 
#include <LittleFS.h> 

const char* ssid = "INDOCENTER";
const char* password = "segosambel";

const String botToken = "7705305354:AAEUi1u8G3JVYd61-6X86006MHWzmgXiCHA";
const String chatId = "-1002896328559";

const uint16_t kIrLed = D5;
IRsend irsend(kIrLed);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 7 * 3600, 60000); // GMT+7

ESP8266WebServer server(80);

uint16_t powerOn[] = {3488, 1696,  434, 1262,  482, 1264,  434, 428,  458, 412,  450, 390,  478, 1270,  434, 402,  460, 474,  396, 1292,  430, 1266,  458, 430,  432, 1264,  458, 432,  460, 442,  400, 1260,  476, 1248,  482, 386,  456, 1294,  460, 1240,  480, 412,  434, 408,  480, 1242,  482, 384,  476, 412,  456, 1270,  430, 470,  424, 410,  430, 418,  478, 448,  422, 410,  452, 388,  486, 384,  482, 384,  460, 432,  454, 412,  432, 466,  398, 434,  460, 412,  434, 414,  480, 410,  456, 410,  430, 470,  398, 406,  482, 384,  456, 434,  434, 1262,  482, 408,  456, 408,  460, 408,  430, 406,  482, 386,  478, 1242,  456, 1264,  480, 384,  454, 412,  480, 386,  458, 434,  434, 430,  432, 406,  456, 432,  460, 406,  434, 432,  434, 404,  458, 406,  478, 388,  480, 1266,  460, 1242,  482, 384,  454, 1302,  434, 1260,  484, 406,  460, 408,  432, 430,  432, 434,  434, 430,  434, 1286,  434, 1264,  478, 1246,  484, 1264,  434, 406,  480, 388,  458, 428,  432, 430,  432, 434,  430, 436,  458, 412,  434, 432,  456, 410,  456, 386,  456, 430,  458, 382,  478, 416,  460, 380,  484, 406,  436, 430,  432, 434,  434, 432,  430, 436,  454, 386,  486, 404,  460, 412,  430, 432,  454, 386,  456, 432,  432, 430,  434, 410,  458, 406,  456, 412,  478, 410,  432, 408,  480, 400,  480, 410,  434, 410,  482, 380,  458, 432,  460, 382,  456, 444,  434, 430,  434, 430,  430, 434,  432, 430,  456, 408,  460, 382,  456, 408,  454, 432,  460, 406,  462, 408,  456, 408,  458, 408,  432, 406,  482, 406,  460, 384,  454, 434,  430, 438,  434, 432,  434, 430,  430, 1292,  456, 1266,  460, 406,  432, 1264,  456, 1292,  432, 1262,  484, 1266,  458, 1264,  434, 16996,  3630, 1692,  460, 1240,  456, 1288,  430, 432,  456, 412,  434, 434,  456, 1244,  480, 386,  480, 406,  434, 1276,  482, 1268,  434, 430,  432, 1288,  432, 410,  456, 432,  456, 1268,  434, 1262,  458, 430,  452, 1248,  484, 1240,  482, 408,  434, 428,  460, 1262,  434, 434,  456, 414,  432, 1292,  434, 430,  454, 414,  432, 430,  456, 412,  432, 434,  458, 408,  458, 406,  462, 406,  458, 410,  458, 384,  482, 406,  432, 434,  460, 408,  460, 410,  456, 394,  458, 434,  460, 418,  460, 408,  434, 428,  454, 410,  434, 1264,  480, 408,  456, 410,  432, 440,  460, 386,  456, 408,  484, 1240,  476, 1272,  460, 406,  456, 412,  456, 384,  478, 410,  432, 432,  460, 406,  456, 386,  458, 406,  454, 436,  432, 434,  432, 434,  432, 404,  456, 1272,  480, 1266,  432, 432,  432, 1266,  486, 1238,  478, 414,  452, 388,  456, 432,  434, 464,  400, 428,  458, 1242,  458, 1262,  480, 1244,  456, 1324,  422, 408,  432, 434,  462, 384,  458, 464,  424, 444,  398, 466,  398, 408,  456, 458,  434, 382,  456, 408,  452, 470,  400, 412,  456, 408,  456, 466,  398, 464,  400, 404,  456, 434,  432, 406,  480, 442,  396, 468,  422, 390,  458, 406,  456, 410,  478, 412,  432, 406,  456, 410,  456, 408,  458, 404,  456, 432,  434, 464,  426, 440,  400, 414,  454, 412,  456, 468,  398, 408,  458, 406,  482, 382,  460, 464,  398, 408,  456, 470,  398, 464,  424, 442,  396, 476,  424, 384,  460, 402,  458, 408,  462, 464,  396, 470,  402, 404,  456, 410,  460, 404,  458, 414,  454, 408,  460, 406,  458, 420,  456, 468,  402, 406,  458, 1258,  488, 1238,  458, 402,  482, 1246,  454, 1268,  486, 1240,  480, 1246,  458, 1266,  452};
uint16_t powerOff[] = {3486, 1696,  436, 1286,  460, 1240,  456, 432,  434, 434,  452, 414,  434, 1288,  434, 430,  458, 412,  436, 1286,  434, 1290,  432, 430,  432, 1292,  430, 432,  458, 412,  432, 1290,  436, 1288,  458, 410,  432, 1290,  460, 1266,  456, 408,  434, 434,  434, 1260,  482, 412,  432, 430,  458, 1244,  454, 434,  460, 410,  430, 442,  432, 436,  452, 414,  456, 410,  454, 416,  432, 434,  432, 434,  436, 430,  434, 428,  430, 436,  456, 416,  432, 444,  430, 432,  452, 414,  434, 432,  434, 432,  432, 432,  432, 430,  434, 434,  432, 432,  434, 428,  458, 408,  434, 436,  458, 410,  434, 1286,  434, 1288,  458, 412,  432, 436,  430, 436,  434, 432,  436, 428,  436, 430,  432, 436,  434, 430,  434, 432,  432, 434,  432, 432,  432, 436,  432, 1288,  430, 1294,  432, 432,  432, 1290,  432, 1288,  456, 414,  432, 434,  432, 434,  432, 428,  434, 430,  434, 1288,  456, 1270,  458, 1272,  434, 1288,  428, 440,  432, 432,  458, 408,  432, 432,  430, 438,  430, 436,  432, 432,  436, 430,  428, 440,  456, 410,  430, 414,  480, 410,  432, 434,  434, 428,  434, 430,  432, 434,  432, 434,  434, 432,  456, 410,  434, 430,  456, 412,  434, 434,  432, 434,  432, 464,  400, 468,  400, 434,  430, 432,  428, 442,  430, 464,  422, 412,  432, 434,  432, 466,  396, 434,  458, 444,  396, 434,  428, 440,  434, 434,  428, 438,  434, 432,  432, 428,  430, 438,  434, 432,  432, 432,  432, 432,  434, 464,  424, 442,  400, 466,  398, 432,  430, 434,  428, 438,  432, 436,  434, 464,  398, 436,  432, 430,  434, 428,  430, 430,  432, 434,  432, 1290,  434, 1286,  434, 470,  396, 1326,  424, 1304,  424, 414,  460, 1308,  402, 1286,  428, 17004,  3620, 1702,  436, 1322,  400, 1288,  434, 430,  432, 466,  398, 462,  426, 1304,  398, 434,  432, 468,  394, 1296,  456, 1268,  434, 462,  392, 1328,  422, 446,  398, 466,  398, 1292,  434, 1322,  396, 470,  400, 1322,  400, 1286,  432, 470,  396, 464,  400, 1294,  430, 434,  434, 428,  434, 1290,  432, 434,  432, 430,  434, 426,  432, 466,  398, 434,  458, 416,  434, 462,  392, 474,  400, 466,  400, 430,  434, 430,  434, 430,  428, 438,  432, 466,  400, 428,  456, 412,  432, 472,  424, 444,  398, 434,  430, 434,  434, 432,  428, 438,  430, 434,  434, 432,  434, 430,  432, 434,  434, 1288,  434, 1286,  430, 436,  456, 444,  396, 436,  454, 414,  434, 430,  434, 430,  430, 436,  430, 436,  456, 442,  422, 420,  434, 430,  428, 438,  434, 1286,  430, 1296,  458, 414,  432, 1290,  428, 1274,  458, 430,  434, 426,  456, 446,  398, 434,  434, 428,  436, 1288,  464, 1264,  434, 1286,  432, 1290,  434, 430,  458, 410,  432, 432,  432, 434,  434, 428,  430, 436,  436, 430,  426, 440,  430, 436,  432, 436,  434, 428,  432, 434,  454, 412,  432, 432,  432, 472,  424, 414,  430, 434,  432, 436,  432, 432,  436, 428,  428, 472,  398, 432,  434, 430,  432, 434,  436, 432,  432, 430,  430, 440,  430, 438,  430, 432,  430, 434,  456, 412,  460, 444,  422, 420,  430, 472,  422, 422,  456, 412,  434, 430,  432, 432,  434, 430,  428, 440,  432, 434,  434, 432,  432, 434,  432, 436,  432, 434,  434, 428,  460, 416,  434, 432,  434, 428,  434, 428,  432, 432,  434, 430,  434, 434,  432, 430,  430, 438,  432, 452,  410, 432,  458, 1268,  434, 1284,  434, 428,  428, 1298,  432, 1292,  436, 428,  434, 1288,  430, 1292,  454};
uint16_t tempUp[] = {3490, 1666,  464, 1262,  456, 1262,  464, 404,  462, 402,  458, 414,  458, 1260,  462, 408,  482, 384,  464, 1262,  462, 1254,  456, 414,  464, 1252,  456, 414,  460, 414,  464, 1258,  464, 1260,  462, 400,  484, 1238,  458, 1268,  462, 404,  458, 408,  466, 1256,  464, 404,  460, 400,  460, 1262,  466, 398,  464, 398,  458, 408,  456, 408,  464, 402,  458, 406,  460, 410,  460, 408,  458, 404,  462, 400,  460, 404,  458, 410,  464, 400,  462, 400,  460, 402,  464, 402,  458, 410,  462, 398,  462, 404,  460, 400,  460, 1264,  462, 402,  462, 400,  460, 406,  464, 402,  464, 402,  462, 1260,  462, 1254,  464, 398,  464, 398,  462, 402,  454, 1268,  462, 406,  462, 398,  462, 402,  462, 400,  458, 412,  464, 402,  462, 402,  484, 386,  488, 1244,  462, 1256,  464, 404,  460, 1258,  464, 1252,  464, 400,  456, 412,  464, 398,  464, 406,  456, 402,  456, 1266,  466, 1256,  460, 1268,  462, 1260,  464, 404,  458, 402,  458, 410,  464, 404,  464, 396,  460, 402,  462, 402,  454, 416,  462, 404,  456, 410,  464, 404,  462, 404,  456, 408,  486, 384,  464, 400,  464, 400,  462, 404,  460, 400,  454, 414,  458, 408,  460, 406,  462, 396,  456, 410,  486, 384,  458, 410,  462, 402,  460, 404,  460, 402,  454, 414,  462, 404,  462, 402,  460, 404,  464, 400,  462, 402,  462, 400,  460, 402,  484, 386,  460, 402,  458, 410,  458, 402,  486, 380,  462, 404,  464, 404,  460, 408,  458, 406,  462, 398,  462, 400,  464, 398,  460, 404,  462, 400,  458, 406,  458, 408,  456, 412,  462, 406,  460, 400,  460, 406,  460, 406,  460, 408,  460, 404,  462, 1256,  464, 1254,  486, 1240,  466, 1256,  458, 1262,  466, 1258,  464, 16990,  3630, 1670,  464, 1262,  460, 1258,  460, 402,  462, 398,  460, 404,  462, 1268,  460, 404,  462, 402,  464, 1262,  460, 1256,  482, 384,  460, 1266,  462, 398,  460, 408,  454, 1266,  462, 1262,  462, 406,  458, 1262,  460, 1268,  458, 410,  462, 408,  458, 1258,  484, 390,  460, 398,  458, 1274,  460, 402,  462, 404,  462, 464,  404, 408,  458, 402,  464, 400,  464, 402,  462, 404,  460, 404,  454, 416,  458, 400,  486, 386,  464, 402,  462, 406,  456, 400,  462, 402,  460, 410,  458, 404,  458, 406,  456, 406,  464, 1256,  454, 412,  458, 408,  462, 402,  458, 410,  460, 404,  456, 1268,  458, 1266,  460, 408,  460, 402,  462, 396,  466, 1260,  460, 406,  460, 402,  482, 388,  460, 404,  462, 404,  458, 408,  462, 406,  460, 408,  462, 1254,  460, 1266,  462, 400,  458, 1266,  460, 1268,  460, 408,  462, 408,  462, 402,  462, 398,  464, 404,  456, 1262,  458, 1268,  484, 1246,  462, 1264,  458, 404,  456, 410,  462, 406,  458, 406,  460, 400,  462, 402,  464, 408,  456, 402,  456, 422,  462, 404,  484, 394,  458, 408,  464, 402,  460, 404,  460, 404,  456, 410,  462, 404,  462, 406,  460, 408,  460, 404,  460, 412,  454, 404,  486, 390,  462, 406,  462, 398,  462, 400,  462, 406,  460, 402,  460, 410,  460, 400,  458, 412,  458, 404,  460, 404,  456, 410,  456, 416,  458, 402,  464, 402,  454, 410,  464, 402,  458, 412,  458, 400,  460, 406,  458, 410,  462, 404,  462, 404,  462, 406,  462, 408,  462, 406,  458, 400,  458, 412,  460, 396,  460, 408,  468, 406,  456, 410,  460, 408,  484, 382,  458, 408,  464, 404,  458, 410,  464, 1260,  464, 1260,  458, 1260,  462, 1256,  486, 1238,  460, 1264,  466};
uint16_t tempDown[] = {3462, 1694,  462, 1260,  460, 1270,  458, 410,  462, 402,  464, 398,  460, 1274,  464, 400,  462, 434,  434, 1254,  460, 1264,  458, 408,  466, 1258,  460, 406,  458, 408,  460, 1260,  464, 1258,  466, 398,  464, 1256,  464, 1256,  466, 400,  462, 400,  462, 1270,  456, 410,  464, 402,  458, 1264,  462, 402,  462, 402,  454, 414,  452, 412,  456, 408,  456, 408,  456, 412,  458, 408,  452, 414,  456, 408,  458, 408,  464, 398,  458, 406,  466, 400,  458, 408,  462, 406,  458, 408,  462, 402,  486, 382,  488, 384,  466, 1256,  464, 400,  450, 414,  466, 398,  464, 398,  454, 414,  456, 1266,  452, 1272,  456, 410,  460, 408,  456, 412,  456, 410,  458, 408,  458, 406,  458, 408,  464, 402,  456, 412,  490, 382,  464, 400,  458, 408,  466, 1256,  456, 1268,  462, 400,  462, 1260,  464, 1270,  452, 414,  464, 402,  458, 410,  466, 400,  464, 398,  464, 1260,  452, 1270,  464, 1258,  466, 398,  454, 1266,  466, 400,  464, 398,  460, 408,  452, 412,  464, 398,  462, 402,  466, 396,  462, 404,  460, 402,  454, 416,  462, 400,  464, 402,  464, 398,  458, 410,  454, 412,  450, 414,  492, 382,  488, 384,  464, 398,  480, 388,  464, 398,  464, 398,  466, 398,  452, 414,  458, 410,  466, 396,  482, 386,  464, 398,  464, 404,  466, 398,  464, 428,  432, 402,  462, 400,  454, 414,  458, 410,  458, 410,  456, 410,  452, 412,  452, 412,  462, 402,  458, 408,  464, 400,  458, 408,  462, 410,  458, 412,  458, 406,  454, 412,  484, 386,  454, 412,  464, 404,  456, 408,  484, 382,  462, 404,  456, 414,  454, 412,  466, 396,  456, 1268,  466, 1254,  466, 398,  458, 1266,  464, 1260,  460, 1264,  462, 404,  462, 404,  462, 17016,  3646, 1670,  466, 1256,  462, 1256,  466, 398,  462, 402,  460, 404,  456, 1264,  462, 406,  458, 406,  460, 1264,  458, 1264,  456, 410,  462, 1258,  462, 402,  464, 402,  462, 1260,  464, 1258,  454, 414,  462, 1260,  462, 1258,  460, 406,  462, 404,  464, 1256,  462, 400,  456, 408,  460, 1260,  466, 398,  456, 410,  462, 400,  458, 408,  460, 406,  456, 410,  462, 400,  458, 410,  464, 400,  454, 414,  462, 406,  462, 398,  464, 414,  462, 406,  460, 402,  466, 396,  464, 398,  454, 414,  462, 406,  464, 396,  456, 1264,  454, 416,  458, 406,  450, 416,  460, 404,  484, 384,  458, 1266,  462, 1266,  466, 398,  464, 400,  464, 400,  466, 398,  454, 410,  456, 408,  458, 412,  464, 402,  452, 412,  458, 408,  464, 400,  452, 416,  456, 1264,  460, 1266,  462, 404,  466, 1256,  464, 1254,  466, 400,  456, 410,  456, 410,  464, 400,  466, 400,  456, 1270,  464, 1264,  462, 1258,  458, 408,  454, 1266,  458, 410,  458, 408,  458, 410,  464, 404,  450, 414,  458, 408,  460, 404,  464, 400,  454, 412,  464, 398,  462, 404,  460, 406,  458, 408,  456, 410,  462, 402,  450, 416,  456, 410,  462, 406,  460, 404,  452, 414,  464, 400,  462, 400,  462, 400,  462, 400,  450, 416,  452, 412,  466, 400,  464, 400,  452, 414,  456, 412,  466, 398,  456, 420,  456, 412,  462, 402,  456, 408,  456, 412,  458, 406,  454, 412,  464, 402,  464, 400,  462, 406,  464, 402,  458, 406,  458, 406,  456, 416,  464, 398,  454, 412,  464, 398,  462, 400,  466, 402,  466, 398,  464, 400,  454, 412,  464, 400,  456, 424,  456, 410,  464, 1258,  456, 1266,  460, 410,  466, 1254,  466, 1256,  464, 1258,  464, 400,  464, 400,  458};

bool isPowerOn = false; 
int currentTemp = 24; 

struct {
  bool enableOnTimer = false;
  bool enableOffTimer = false;
  int onHour = 0;
  int onMinute = 0;
  int offHour = 0;
  int offMinute = 0;
  bool daysActiveOn[8] = {false}; 
  bool daysActiveOff[8] = {false};
} acTimer;

unsigned long lastMemoryCheck = 0;

#define EEPROM_SIZE 128
#define TIMER_START_ADDR 0
#define MEMORY_CHECK_INTERVAL 30000
#define MEMORY_CRITICAL_THRESHOLD 3000
#define EEPROM_USAGE_THRESHOLD 0.9 
#define FLASH_CHECK_INTERVAL 3600000 
#define FLASH_USAGE_THRESHOLD 0.85 
#define LOG_FILE "/log.txt"
#define MAX_LOG_SIZE 10240
#define EEPROM_TEMP_ADDR 64

const char* getDayName(int day);
const char* getMonthName(int month);
String getDateTimeString();
String urlencode(String str);
void sendTelegramMessage(String message);
void sendIRCommand(uint16_t* data, size_t length);
void togglePower(bool state);
void checkTimer();
String getTimeString(int hour, int minute);
String getTimerStatus();
void printMemoryInfo();
String getFullTimeString();
void handleUpdateTelegram();
void handleRoot();
void handleToggle();
void handleTempUp();
void handleTempDown();
void handleSetTimer();
void logEvent(String message);
void rotateLogs();
String readLogs();
String readBotToken();
void checkFlashUsage();
void cleanFlashIfNeeded();
byte calculateChecksum();
void saveTimerToEEPROM();
void loadTimerFromEEPROM();
void checkEEPROMUsage();
void formatEEPROM();

unsigned long lastFlashCheck = 0;

void saveTemperature() {
  EEPROM.begin(512);
  EEPROM.write(EEPROM_TEMP_ADDR, currentTemp);
  EEPROM.commit();
  EEPROM.end();
}

void loadTemperature() {
  EEPROM.begin(512);
  currentTemp = EEPROM.read(EEPROM_TEMP_ADDR);
  // Validasi nilai
  if(currentTemp < 16 || currentTemp > 30) currentTemp = 24; // Nilai default jika invalid
  EEPROM.end();
}

void logEvent(String message) {
  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS for logging");
    return;
  }

  rotateLogs();

  File logFile = LittleFS.open(LOG_FILE, "a");
  if (!logFile) {
    Serial.println("Failed to open log file");
    return;
  }

  String timestamp = getDateTimeString();
  logFile.println("[" + timestamp + "] " + message);
  logFile.close();
  
  Serial.println("Logged: " + message);
}

void rotateLogs() {
  if (LittleFS.exists(LOG_FILE)) {
    File logFile = LittleFS.open(LOG_FILE, "r");
    if (logFile.size() > MAX_LOG_SIZE) {
      LittleFS.remove(LOG_FILE);
      logEvent("Log file rotated");
    }
    logFile.close();
  }
}

String readBotToken() {
  EEPROM.begin(512);
  String token;
  for (int i = 0; i < 64; i++) {
    char c = EEPROM.read(i);
    if (c == 0) break;
    token += c;
  }
  EEPROM.end();
  return token;
}

String readLogs() {
  if (!LittleFS.begin()) {
    return "Failed to initialize LittleFS";
  }

  if (!LittleFS.exists(LOG_FILE)) {
    return "No log entries yet";
  }

  File logFile = LittleFS.open(LOG_FILE, "r");
  if (!logFile) {
    return "Failed to open log file";
  }

  String logs;
  while (logFile.available()) {
    logs += logFile.readStringUntil('\n') + "\n";
  }
  logFile.close();
  
  return logs;
}

void checkFlashUsage() {
  FSInfo fs_info;
  if (!LittleFS.info(fs_info)) {
    Serial.println("Gagal membaca info filesystem");
    return;
  }

  Serial.println("\n=== Flash Memory Info ===");
  Serial.printf("Total Space: %10d bytes\n", fs_info.totalBytes);
  Serial.printf("Used Space:  %10d bytes\n", fs_info.usedBytes);
  Serial.printf("Block Size:  %10d bytes\n", fs_info.blockSize);
  Serial.printf("Page Size:   %10d bytes\n", fs_info.pageSize);
  Serial.printf("Usage:       %8.1f%%\n", (fs_info.usedBytes * 100.0) / fs_info.totalBytes);

  if (fs_info.usedBytes > fs_info.totalBytes * FLASH_USAGE_THRESHOLD) {
    Serial.println("Peringatan: Flash memory hampir penuh!");
    sendTelegramMessage("⚠️ Peringatan: Flash memory hampir penuh (" + 
                      String((fs_info.usedBytes * 100) / fs_info.totalBytes) + "%)");
  }
}

void cleanFlashIfNeeded() {
  FSInfo fs_info;
  LittleFS.info(fs_info);
  
  if (fs_info.usedBytes > fs_info.totalBytes * 0.9) {
    Serial.println("Membersihkan flash memory...");
    LittleFS.format();
    Serial.println("Flash memory telah diformat ulang");
    sendTelegramMessage("♻️ Flash memory telah diformat ulang");
  }
}

byte calculateChecksum() {
  byte checksum = 0;
  for (size_t i = 0; i < sizeof(acTimer)-1; i++) {
    checksum ^= ((byte*)&acTimer)[i];
  }
  return checksum;
}

void saveTimerToEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.put(TIMER_START_ADDR, acTimer);
  EEPROM.commit();
  EEPROM.end();
  Serial.println("Timer disimpan ke EEPROM");
}

void loadTimerFromEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(TIMER_START_ADDR, acTimer);
  
  if (acTimer.onHour > 23 || acTimer.onMinute > 59 || 
      acTimer.offHour > 23 || acTimer.offMinute > 59) {
    memset(&acTimer, 0, sizeof(acTimer));
    Serial.println("EEPROM corrupt, reset timer");
  }
  EEPROM.end();
}

void checkEEPROMUsage() {
  EEPROM.begin(EEPROM_SIZE);
  
  int usedBytes = 0;
  for (int i = 0; i < EEPROM_SIZE; i++) {
    if (EEPROM.read(i) != 0xFF) {
      usedBytes++;
    }
  }
  EEPROM.end();

  Serial.printf("Penggunaan EEPROM: %d/%d bytes (%.1f%%)\n", 
               usedBytes, EEPROM_SIZE, (usedBytes * 100.0) / EEPROM_SIZE);

  if (usedBytes > EEPROM_SIZE * EEPROM_USAGE_THRESHOLD) {
    Serial.println("EEPROM hampir penuh, melakukan reset...");
    sendTelegramMessage("⚠️ Peringatan: EEPROM hampir penuh, sistem akan mereset");
    formatEEPROM();
    ESP.restart();
  }
}

void formatEEPROM() {
  EEPROM.begin(EEPROM_SIZE);
  for (int i = 0; i < EEPROM_SIZE; i++) {
    EEPROM.write(i, 0xFF);
  }
  EEPROM.commit();
  EEPROM.end();
  Serial.println("EEPROM telah direset");
}

String getFullTimeString() {
  timeClient.update();
  int h = hour();
  int m = minute();
  return getTimeString(h, m) + " WIB";
}

void printMemoryInfo() {
  Serial.println("\n=== Memory Info ===");
  Serial.printf("Free Heap: %6d bytes\n", ESP.getFreeHeap());
  Serial.printf("Heap Frag: %6d%%\n", ESP.getHeapFragmentation());
  Serial.printf("Max Block: %6d bytes\n", ESP.getMaxFreeBlockSize());
  Serial.printf("Chip ID: %08X\n", ESP.getChipId());
  Serial.println("===================");
}

const char* getDayName(int day) {
  const char* days[] = {"", "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
  return (day >= 1 && day <= 7) ? days[day] : "";
}

const char* getMonthName(int month) {
  const char* months[] = {"", "Jan", "Feb", "Mar", "Apr", "Mei", "Jun", "Jul", "Agu", "Sep", "Okt", "Nov", "Des"};
  return (month >= 1 && month <= 12) ? months[month] : "";
}

String getDateTimeString() {
  timeClient.update();
  int wday = weekday();
  int d = day();
  int m = month();
  int y = year();
  int h = hour();
  int min = minute();

  String waktu = String(getDayName(wday)) + ", " + String(d) + " " + String(getMonthName(m)) + " " + String(y);
  waktu += " ";
  if (h < 10) waktu += "0";
  waktu += String(h) + ":";
  if (min < 10) waktu += "0";
  waktu += String(min);
  return waktu;
}

String urlencode(String str) {
  String encoded = "";
  char c;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) encoded += c;
    else if (c == ' ') encoded += '+';
    else {
      char hex[4];
      sprintf(hex, "%%%02X", c);
      encoded += hex;
    }
  }
  return encoded;
}

void sendTelegramMessage(String message) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Tidak terhubung ke WiFi, tidak bisa mengirim pesan Telegram");
    return;
  }

  String url = "https://api.telegram.org/bot" + botToken +
               "/sendMessage?chat_id=" + chatId +
               "&text=" + urlencode(message);

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  if (https.begin(client, url)) {
    int httpCode = https.GET();
    Serial.print("Telegram sent, code: ");
    Serial.println(httpCode);

    int retryCount = 0;
    while (retryCount < 3 && httpCode != 200) {
      delay(1000);
      httpCode = https.GET();
      retryCount++;
      Serial.print("Retry ");
      Serial.print(retryCount);
      Serial.println(", HTTP code: " + String(httpCode));
    }

    if (httpCode != 200) {
      String payload = https.getString();
      Serial.println("Response: " + payload);
    }
    https.end();
  } else {
    Serial.println("Failed to connect to Telegram API");
  }
}

void sendIRCommand(uint16_t* data, size_t length) {
  if (length == 0) {
    Serial.println("Kode IR kosong!");
    return;
  }
  irsend.sendRaw(data, length, 38);
  delay(200);
}

void togglePower(bool state) {
  if (state) {
    sendIRCommand(powerOn, sizeof(powerOn) / sizeof(powerOn[0]));
    isPowerOn = true;
    Serial.println("AC Nyala");
    sendTelegramMessage("🔔 AC telah dinyalakan");
    logEvent("AC turned ON. Temperature: " + String(currentTemp) + "°C");
  } else {
    sendIRCommand(powerOff, sizeof(powerOff) / sizeof(powerOff[0]));
    isPowerOn = false;
    logEvent("AC turned OFF");
    Serial.println("AC Mati");
    sendTelegramMessage("❄ AC telah dimatikan");
  }
}

void handleUpdateTelegram() {
  if (WiFi.status() != WL_CONNECTED) return;

  WiFiClientSecure client;
  client.setInsecure();
  
  HTTPClient https;
  String url = "https://api.telegram.org/bot" + botToken + "/getUpdates";
  
  if (https.begin(client, url)) {
    int httpCode = https.GET();
    
    if (httpCode == 200) {
      String payload = https.getString();
      DynamicJsonDocument doc(512);
      deserializeJson(doc, payload);

      if (doc["result"].size() > 0) {
        String messageText = doc["result"][0]["message"]["text"].as<String>();
        String chatId = doc["result"][0]["message"]["chat"]["id"].as<String>();
        int updateId = doc["result"][0]["update_id"].as<int>();

        if (messageText == "/status") {
          String reply;
          if (isPowerOn) {
            reply = "⚡ Status AC: ON\n🌡 Suhu: " + String(currentTemp) + "°C";
          } else {
            reply = "⚡ Status AC: OFF";
          }
          
          if (acTimer.enableOnTimer || acTimer.enableOffTimer) {
            reply += "\n\n⏰ Timer:\n" + getTimerStatus();
          }

          String urlReply = "https://api.telegram.org/bot" + botToken + 
                         "/sendMessage?chat_id=" + chatId + 
                         "&text=" + urlencode(reply);
                         
          if (https.begin(client, urlReply)) {
            https.GET();
            https.end();
          }
        }

        String urlDelete = "https://api.telegram.org/bot" + botToken + 
                         "/getUpdates?offset=" + String(updateId + 1);
        https.begin(client, urlDelete);
        https.GET();
        https.end();
      }
    }
    https.end();
  }
}

void checkTimer() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  setTime(epochTime);

  int currentHour = hour();
  int currentMinute = minute();
  int currentSecond = second();
  int currentDay = weekday();

  if (acTimer.enableOnTimer && currentHour == acTimer.onHour && currentMinute == acTimer.onMinute && currentSecond == 0) {
    if (acTimer.daysActiveOn[currentDay] && !isPowerOn) {
      togglePower(true);
      String notif = "⏰ AC dinyalakan oleh timer!\n";
      notif += "🕒 Waktu: " + getTimeString(currentHour, currentMinute) + "\n";
      notif += "📅 Hari: " + String(getDayName(currentDay));
      sendTelegramMessage(notif);
    }
  }
  
  if (acTimer.enableOffTimer && currentHour == acTimer.offHour && currentMinute == acTimer.offMinute && currentSecond == 0) {
    if (acTimer.daysActiveOff[currentDay] && isPowerOn) {
      togglePower(false);
      String notif = "⏰ AC dimatikan oleh timer!\n";
      notif += "🕒 Waktu: " + getTimeString(currentHour, currentMinute) + "\n";
      notif += "📅 Hari: " + String(getDayName(currentDay));
      sendTelegramMessage(notif);
    }
  }
}

String getTimeString(int hour, int minute) {
  char timeStr[6];
  sprintf(timeStr, "%02d:%02d", hour, minute);
  return String(timeStr);
}

String getTimerStatus() {
  String status = "";
  const char* hari[] = {"", "Min", "Sen", "Sel", "Rab", "Kam", "Jum", "Sab"};

  if (acTimer.enableOnTimer) {
    status += "ON: " + getTimeString(acTimer.onHour, acTimer.onMinute) + " (";
    bool adaHari = false;
    for (int i = 1; i <= 7; i++) {
      if (acTimer.daysActiveOn[i]) {
        if (adaHari) status += ", ";
        status += hari[i];
        adaHari = true;
      }
    }
    status += adaHari ? ")" : "-)";
  }
  
  if (acTimer.enableOffTimer) {
    if (!status.isEmpty()) status += " | ";
    status += "OFF: " + getTimeString(acTimer.offHour, acTimer.offMinute) + " (";
    bool adaHari = false;
    for (int i = 1; i <= 7; i++) {
      if (acTimer.daysActiveOff[i]) {
        if (adaHari) status += ", ";
        status += hari[i];
        adaHari = true;
      }
    }
    status += adaHari ? ")" : "-)";
  }
  
  return status.isEmpty() ? "Timer tidak aktif" : status;
}

void handleRoot() {
  String acStatusColor = isPowerOn ? "#00e676" : "#f44336";

  String html = "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no\">"
                "<title>Remote AC</title>"
                "<style>"
                "@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@400;600&display=swap');"
                "body {"
                "  font-family: 'Poppins', sans-serif;"
                "  background: url('https://i.imgur.com/ZP3gWKh.png') no-repeat center center fixed;"
                "  background-size: cover;"
                "  color: #ffffff;"
                "  margin: 0;"
                "  padding: 20px;"
                "  text-align: center;"
                "}"
                ".container { max-width: 800px; margin: auto; }"
                ".status-box, .timer-box {"
                "  background: rgba(0, 40, 60, 0.6);"
                "  border-radius: 12px;"
                "  padding: 20px;"
                "  margin: 20px 0;"
                "  box-shadow: 0 8px 24px rgba(0,0,0,0.4);"
                "}"
                ".power-btn {"
                "  background: linear-gradient(45deg, #2193b0, #6dd5ed);"
                "  color: white;"
                "  border: none;"
                "  border-radius: 12px;"
                "  padding: 16px;"
                "  font-size: 20px;"
                "  width: 100%;"
                "  margin: 12px 0;"
                "  cursor: pointer;"
                "  transition: background 0.4s, transform 0.2s;"
                "}"
                ".power-btn:hover {"
                "  background: linear-gradient(45deg, #1e88e5, #90caf9);"
                "  transform: scale(1.04);"
                "}"
                ".temp-btn {"
                "  background: linear-gradient(90deg, #00c6ff, #0072ff);"
                "  color: white;"
                "  border: none;"
                "  border-radius: 10px;"
                "  padding: 12px;"
                "  font-size: 16px;"
                "  width: 100%;"
                "  margin: 6px 0;"
                "  cursor: pointer;"
                "  transition: background 0.3s, transform 0.2s;"
                "}"
                ".temp-btn:hover {"
                "  background: linear-gradient(90deg, #0072ff, #00c6ff);"
                "  transform: scale(1.05);"
                "}"
                ".temp-display {"
                "  font-size: 34px;"
                "  font-weight: bold;"
                "  margin: 15px 0;"
                "  color: #ffffff;"
                "  text-shadow: 1px 1px 4px #000;"
                "}"
                ".timer-box {"
                "  padding: 15px;"
                "  margin: 10px 0;"
                "  box-shadow: 0 6px 15px rgba(0, 0, 0, 0.2);"
                "  text-align: left;"
                "}"
                "input[type=\"time\"] {"
                "  width: 97%;"
                "  padding: 12px;"
                "  margin: 10px 0;"
                "  border: 1px solid #ccc;"
                "  border-radius: 6px;"
                "  background: rgba(255,255,255,0.9);"
                "}"
                ".checkbox-label {"
                "  display: block;"
                "  margin: 5px 0;"
                "  color: #ffffff;"
                "}"
                ".timer-status {"
                "  background: rgba(255, 255, 255, 0.2);"
                "  padding: 10px;"
                "  border-radius: 8px;"
                "  margin: 10px 0;"
                "  font-size: 15px;"
                "  color: #ffffff;"
                "}"
                "</style>"
                "</head>"
                "<body>"
                "<div style='display: flex; align-items: center; justify-content: center; gap: 12px; margin-bottom: 20px;'>"
                "<h1 style='font-size: 28px; font-weight: 600; color: white; margin: 0; filter: drop-shadow(0 0 6px rgba(0,0,0,0.5));'>Remote AC</h1>"
                "<img src='https://i.imgur.com/A22146X.png' alt='Radnet Logo' style='height: 60px; filter: drop-shadow(0 0 6px rgba(0,0,0,0.5));'>"
                "</div>"
                "<div class='container'>"
                "<div class='status-box'>"
                "<p id='clock'>" + getTimeString(hour(), minute()) + ":" + String(second()) + "</p>"
                "<div style='color:" + acStatusColor + "; font-weight:bold;'>Status AC : " + String(isPowerOn ? "ON" : "OFF") + "</div>"
                "</div>"
                "<button class='power-btn' onclick=\"location.href='/toggle'\">" + String(isPowerOn ? "MATIKAN AC" : "NYALAKAN AC") + "</button>"
                "<div class='status-box'>"
                "<div class='temp-display'>Temperature AC Pada Remote : " + String(currentTemp) + "&deg;C</div>"
                "<button class='temp-btn' onclick=\"location.href='/tempup'\">NAIKKAN SUHU &#9650;</button>"
                "<button class='temp-btn' onclick=\"location.href='/tempdown'\">TURUNKAN SUHU &#9660;</button>"
                "</div>"
                "<div class='timer-box'>"
                "<h3>Timer AC</h3>"
                "<div class=\"timer-status\">" + getTimerStatus() + "</div>"
                "<form action=\"/settimer\" method=\"POST\">"
                "<label>Nyalakan AC:</label>"
                "<input type=\"time\" name=\"ontime\" value=\"" + getTimeString(acTimer.onHour, acTimer.onMinute) + "\">"
                "<label class=\"checkbox-label\">"
                "<input type=\"checkbox\" name=\"enableon\"" + String(acTimer.enableOnTimer ? " checked" : "") + "> Aktifkan timer</label>"
                "<div><b>Set Hari :</b><br>";

  const char* hariFull[] = {"", "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"};
  for (int i = 1; i <= 7; i++) {
    html += "<label class=\"checkbox-label\"><input type=\"checkbox\" name=\"dayon" + String(i) + "\"" + 
            String(acTimer.daysActiveOn[i] ? " checked" : "") + "> " + String(hariFull[i]) + "</label>";
  }

  html += "</div><br>"
          "<label>Matikan AC:</label>"
          "<input type=\"time\" name=\"offtime\" value=\"" + getTimeString(acTimer.offHour, acTimer.offMinute) + "\">"
          "<label class=\"checkbox-label\">"
          "<input type=\"checkbox\" name=\"enableoff\"" + String(acTimer.enableOffTimer ? " checked" : "") + "> Aktifkan timer</label>"
          "<div><b>Set Hari :</b><br>";

  for (int i = 1; i <= 7; i++) {
    html += "<label class=\"checkbox-label\"><input type=\"checkbox\" name=\"dayoff" + String(i) + "\"" + 
            String(acTimer.daysActiveOff[i] ? " checked" : "") + "> " + String(hariFull[i]) + "</label>";
  }

  html += "</div><br>"
          "<button type=\"submit\" class=\"temp-btn\">SIMPAN TIMER</button>"
          "</form>"
          "</div>"
          "</div>"
          "<script>"
          "function updateClock() {"
          "  const now = new Date();"
          "  now.setHours(now.getHours());"
          "  let jam = now.getHours().toString().padStart(2, '0');"
          "  let menit = now.getMinutes().toString().padStart(2, '0');"
          "  let detik = now.getSeconds().toString().padStart(2, '0');"
          "  document.getElementById('clock').innerHTML = 'Real Time WIB : ' + jam + ':' + menit + ':' + detik;"
          "}"
          "setInterval(updateClock, 1000);"
          "updateClock();"
          "</script>"
          "</body>"
          "</html>";

  server.send(200, "text/html", html);
}

void handleToggle() {
  togglePower(!isPowerOn);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleTempUp() {
  if (currentTemp < 30) {
    currentTemp++;
    sendIRCommand(tempUp, sizeof(tempUp)/sizeof(tempUp[0]));
    saveTemperature();
    sendTelegramMessage("🌡 Suhu dinaikkan ke " + String(currentTemp) + "°C");
  }
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleTempDown() {
  if (currentTemp > 16) {
    currentTemp--;
    sendIRCommand(tempDown, sizeof(tempDown)/sizeof(tempDown[0]));
    saveTemperature();
    sendTelegramMessage("🌡 Suhu diturunkan ke " + String(currentTemp) + "°C");
  }
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleSetTimer() {
  if (server.method() == HTTP_POST) {
    String onTime = server.arg("ontime");
    String offTime = server.arg("offtime");

    if (onTime.indexOf(":") != -1) {
      acTimer.onHour = onTime.substring(0, onTime.indexOf(":")).toInt();
      acTimer.onMinute = onTime.substring(onTime.indexOf(":")+1).toInt();
    }
    if (offTime.indexOf(":") != -1) {
      acTimer.offHour = offTime.substring(0, offTime.indexOf(":")).toInt();
      acTimer.offMinute = offTime.substring(offTime.indexOf(":")+1).toInt();
    }

    if (acTimer.onHour > 23 || acTimer.onMinute > 59 || acTimer.offHour > 23 || acTimer.offMinute > 59) {
    server.send(400, "text/plain", "Waktu tidak valid (jam: 0-23, menit: 0-59)");
    return;
    }

    acTimer.enableOnTimer = server.hasArg("enableon");
    acTimer.enableOffTimer = server.hasArg("enableoff");

    for (int i = 1; i <= 7; i++) {
      acTimer.daysActiveOn[i] = false;
      acTimer.daysActiveOff[i] = false;
    }

    for (int i = 1; i <= 7; i++) {
      if (server.hasArg("dayon" + String(i))) {
        acTimer.daysActiveOn[i] = true;
      }
    }

    for (int i = 1; i <= 7; i++) {
      if (server.hasArg("dayoff" + String(i))) {
        acTimer.daysActiveOff[i] = true;
      }
    }

    String telegramMsg = "🕒 Timer AC diset:\n";
    
    if (acTimer.enableOnTimer) {
      telegramMsg += "⏰ Nyalakan Ac: " + getTimeString(acTimer.onHour, acTimer.onMinute) + "\n";
      telegramMsg += "📅 Hari: ";
      bool first = true;
      for (int i = 1; i <= 7; i++) {
        if (acTimer.daysActiveOn[i]) {
          if (!first) telegramMsg += ", ";
          telegramMsg += getDayName(i);
          first = false;
        }
      }
      if (first) telegramMsg += "Tidak ada hari yang dipilih";
      telegramMsg += "\n";
    } else {
      telegramMsg += "⏰ Timer nyala: Nonaktif\n";
    }
    
    if (acTimer.enableOffTimer) {
      telegramMsg += "\n⏰ Matikan Ac: " + getTimeString(acTimer.offHour, acTimer.offMinute) + "\n";
      telegramMsg += "📅 Hari: ";
      bool first = true;
      for (int i = 1; i <= 7; i++) {
        if (acTimer.daysActiveOff[i]) {
          if (!first) telegramMsg += ", ";
          telegramMsg += getDayName(i);
          first = false;
        }
      }
      if (first) telegramMsg += "Tidak ada hari yang dipilih";
      telegramMsg += "\n";
    } else {
      telegramMsg += "\n⏰ Timer mati: Nonaktif\n";
    }

    sendTelegramMessage(telegramMsg);
    saveTimerToEEPROM();

    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  }
}

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("\n\n=== SYSTEM STARTING ===");

  EEPROM.begin(EEPROM_SIZE); 
  loadTimerFromEEPROM();     

  irsend.begin();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected, reconnecting...");
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    delay(5000);
  }

  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS");
    logEvent("System startup failed: LittleFS init error");
    ESP.restart();
  }
  
  logEvent("System started");
  logEvent("Connected to WiFi. IP: " + WiFi.localIP().toString());

  if (!LittleFS.format()) {
      Serial.println("Gagal memformat LittleFS!");
      sendTelegramMessage("⚠️ Gagal memformat LittleFS, restarting...");
      delay(1000);
      ESP.restart();
  }
  
  timeClient.begin();
  timeClient.setTimeOffset(7 * 3600);
  timeClient.update();
  Serial.println("NTP initialized: " + getDateTimeString());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/tempup", handleTempUp);
  server.on("/tempdown", handleTempDown);
  server.on("/settimer", HTTP_POST, handleSetTimer);

  checkFlashUsage();
  cleanFlashIfNeeded();
  printMemoryInfo();
  checkEEPROMUsage();
  loadTemperature();

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();

  static unsigned long lastCheck = 0;
  static unsigned long lastNTP = 0;
  static unsigned long lastTelegramCheck = 0;
  static unsigned long lastMemoryCheck = 0;
  unsigned long now = millis();

  if (now - lastNTP >= 60000) {
    lastNTP = now;
    if (WiFi.status() == WL_CONNECTED) {
      timeClient.update();
      setTime(timeClient.getEpochTime());
    } else {
      Serial.println("WiFi disconnected, trying to reconnect...");
      WiFi.reconnect();
    }
  }

  if (now - lastCheck >= 1000) {
    lastCheck = now;
    checkTimer();
  }

  if (now - lastTelegramCheck >= 5000) {
    lastTelegramCheck = now;
    if (WiFi.status() == WL_CONNECTED) {
      handleUpdateTelegram();
    }
  }

  if (millis() - lastFlashCheck > FLASH_CHECK_INTERVAL) {
    lastFlashCheck = millis();
    checkFlashUsage();
    cleanFlashIfNeeded();
  }

  if (now - lastMemoryCheck >= MEMORY_CHECK_INTERVAL) {
    lastMemoryCheck = now;
    
    printMemoryInfo();
    checkEEPROMUsage();

    if (ESP.getFreeHeap() < MEMORY_CRITICAL_THRESHOLD || ESP.getHeapFragmentation() > 50) {
      Serial.println("Memory kritis, melakukan restart...");
      sendTelegramMessage("⚠️ Peringatan: Memory kritis, sistem akan mereset");
      delay(1000);
      ESP.restart();
    }
  }
}