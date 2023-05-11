#ifndef CONFIGURATION_LOADER_H
#define CONFIGURATION_LOADER_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Storage.h"

class Configuration {
   private:
	static char filePath[128];
	static StaticJsonDocument<2048> data;

   public:
	static bool load(const char* filePath);
	static bool save();
	static bool remove();

	static void setWiFiSSID(const char* ssid);
	static size_t getWiFiSSID(char* ssid);

	static void setWiFiPassword(const char* password);
	static size_t getWiFiPassword(char* password);
};

#endif