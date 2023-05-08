#ifndef ENDPOINT_HANDLER_H
#define ENDPOINT_HANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>

#include "storage_handler.h"
#include "utils.h"

void webServerInit(WebServer* server);
void webServerProcess(WebServer* server);

#endif