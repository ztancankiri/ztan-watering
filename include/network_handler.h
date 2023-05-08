#ifndef NETWORK_HANDLER_H
#define NETWORK_HANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

void wifiClientInit(const char* ssid, const char* password);
void wifiAPInit(const char* ssid);
int wifiScan(char* availableNetworks);
void mDNSInit(const char* hostname);

#endif