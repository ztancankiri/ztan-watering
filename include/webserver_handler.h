#ifndef ENDPOINT_HANDLER_H
#define ENDPOINT_HANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>

#include "network_handler.h"
#include "sensor_handler.h"
#include "storage_handler.h"
#include "utils.h"

void webServerInit(WebServer* server, Sensor* sensor);
void webServerProcess(WebServer* server);

#endif