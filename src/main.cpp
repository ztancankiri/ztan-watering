#include <Arduino.h>

#include "Sensor.h"
#include "WebServerEx.h"
#include "network_handler.h"
#include "utils.h"

WebServerEx* webServerEx;
Sensor* sensor;

void setup() {
	Serial.begin(9600);
	storageInit();

	char configBuffer[128], ssid[128], password[128] = {'\0'};

	if (readFile(CONFIG_FILE, configBuffer) > 0) {
		if (!parseConfig(configBuffer, ssid, password)) {
			Serial.println("Could not parse config!");

			if (deleteFile(CONFIG_FILE)) {
				Serial.println("Config is deleted.");
			} else {
				Serial.println("Config is NOT deleted.");
			}

			delay(1000);
			ESP.restart();
		}

		Serial.print("SSID: ");
		Serial.println(ssid);
		Serial.print("Password: ");
		Serial.println(password);

		wifiClientInit(ssid, password);
	} else {
		Serial.println("Config Not Exist!");
		wifiAPInit(AP_SSID);
	}

	mDNSInit("ztan-watering");

	sensor = new Sensor(26, 35);
	webServerEx = new WebServerEx(80, sensor);
}

void loop() {
}