#include "Configuration.h"

char Configuration::filePath[128];
StaticJsonDocument<2048> Configuration::data;

bool Configuration::load(const char* filePath) {
	strcpy(Configuration::filePath, filePath);

	char buffer[1024];

	size_t size = Storage::read(Configuration::filePath, buffer);

	if (size > 0) {
		DeserializationError error = deserializeJson(Configuration::data, buffer);
		return !error;
	}

	return false;
}

bool Configuration::save() {
	String rawData;
	serializeJson(Configuration::data, rawData);
	return Storage::write(Configuration::filePath, rawData.c_str());
}

bool Configuration::remove() {
	return Storage::remove(Configuration::filePath);
}

size_t Configuration::getWiFiSSID(char* ssid) {
	strcpy(ssid, Configuration::data["ssid"].as<const char*>());
	return strlen(ssid);
}

void Configuration::setWiFiSSID(const char* ssid) {
	Configuration::data["ssid"] = ssid;
}

size_t Configuration::getWiFiPassword(char* password) {
	strcpy(password, Configuration::data["password"].as<const char*>());
	return strlen(password);
}

void Configuration::setWiFiPassword(const char* password) {
	Configuration::data["password"] = password;
}