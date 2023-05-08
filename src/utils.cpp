#include "utils.h"

bool parseConfig(char *config, char *ssid, char *password) {
	DynamicJsonDocument doc(128);
	DeserializationError error = deserializeJson(doc, config);

	if (error) {
		return false;
	}

	strcpy(ssid, doc["ssid"].as<const char *>());
	strcpy(password, doc["password"].as<const char *>());

	return true;
}