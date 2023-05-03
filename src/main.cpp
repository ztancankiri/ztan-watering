#include <Arduino.h>

#include "network_handler.h"
#include "utils.h"
#include "webserver_handler.h"

WebServer server(80);
char configBuffer[128], ssid[128], password[128] = {'\0'};

void setup() {
	Serial.begin(9600);
	storageInit();

	if (readFile(CONFIG_FILE, configBuffer) > 0) {
		if (!parseConfig(configBuffer, "|", ssid, password)) {
			Serial.println("Could not parse config!");

			if (deleteFile(CONFIG_FILE)) {
				Serial.println("Config is deleted.");
			} else {
				Serial.println("Config is NOT deleted.");
			}

			delay(1000);
			ESP.restart();
		}

		wifiClientInit(ssid, password);
	} else {
		Serial.println("Config Not Exist!");
		wifiAPInit(AP_SSID);
	}

	mDNSInit("ztan-watering");
	webServerInit(&server);
}

void loop() {
	webServerProcess(&server);
}