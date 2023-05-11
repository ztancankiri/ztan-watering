#include <Arduino.h>

#include "Configuration.h"
#include "Sensor.h"
#include "WebServerEx.h"
#include "network_handler.h"

#define CONFIG_FILE "/config.txt"
#define AP_SSID "Ztan-Watering"

WebServerEx* webServerEx;
Sensor* sensor;

void setup() {
	Serial.begin(9600);
	Storage::mount();

	bool isLoaded = Configuration::load(CONFIG_FILE);

	if (isLoaded) {
		char ssid[128], password[128];
		Configuration::getWiFiSSID(ssid);
		Configuration::getWiFiPassword(password);
		Serial.printf("SSID: %s\nPassword: %s\n", ssid, password);
		wifiClientInit(ssid, password);
	} else {
		Serial.println("Error on loading the configuration from storage.");
		Configuration::remove();
		Serial.println("Switching to AP mode.");
		wifiAPInit(AP_SSID);
	}

	sensor = new Sensor(26, 35);
	webServerEx = new WebServerEx(80, sensor);
	mDNSInit("ztan-watering");
}

void loop() {
}