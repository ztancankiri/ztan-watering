#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

class Network {
   private:
	static StaticJsonDocument<2048> networks;

   public:
	static void clientInit(const char* ssid, const char* password);
	static void APInit(const char* ssid);
	static int scan(char* availableNetworks);
	static void mDNSInit(const char* hostname);
};

#endif