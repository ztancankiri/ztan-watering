#include <Arduino.h>

#include "Configuration.h"
#include "Network.h"
#include "Sensor.h"
#include "WebServerEx.h"

#define CONFIG_FILE "/config.txt"
#define AP_SSID "Ztan-Watering"
#define HOSTNAME "ztan-watering"

#define WEBSERVER_PORT 80
#define DHT_PIN 26
#define MOISTURE_SENSOR_PIN 35

void setup() {
	Serial.begin(9600);
	Storage::mount();

	bool isLoaded = Configuration::load(CONFIG_FILE);

	if (isLoaded) {
		char ssid[128], password[128];
		Configuration::getWiFiSSID(ssid);
		Configuration::getWiFiPassword(password);
		Serial.printf("SSID: %s\nPassword: %s\n", ssid, password);
		Network::clientInit(ssid, password);
	} else {
		Serial.println("Error on loading the configuration from storage.");
		Configuration::remove();
		Serial.println("Switching to AP mode.");
		Network::APInit(AP_SSID);
	}

	new WebServerEx(WEBSERVER_PORT, new Sensor(DHT_PIN, MOISTURE_SENSOR_PIN));
	Network::mDNSInit(HOSTNAME);
}

void loop() {
}