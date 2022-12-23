#ifndef webserver_h
#define webserver_h
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "xinbot.h"

extern int Servo_PROGRAM;
extern int GPIO_ID;
extern String ival;

void webinit();
void enableWebServer();
void handleZero();
void handleSetting();
void handleEditor();
void handleIndex();
void handleOnLine();
void handleController();
void handleSave();
void handleClient();

void writeKeyValue(int8_t key, int8_t value);
int8_t readKeyValue(int8_t key);

#endif
