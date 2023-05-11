#include <Arduino.h>

#include "Configuration.h"
#include "Motor.h"
#include "Network.h"
#include "Sensor.h"
#include "WebServerEx.h"

#define CONFIG_FILE "/config.txt"
#define AP_SSID "Ztan-Watering"
#define HOSTNAME "ztan-watering"

#define WEBSERVER_PORT 80
#define DHT_PIN 26
#define MOISTURE_SENSOR_PIN 35
#define MOTOR_PIN 13

Sensor* sensor = NULL;
Motor* motor = NULL;

void setup() {
	Serial.begin(9600);
	Storage::mount();

	if (Configuration::load(CONFIG_FILE)) {
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

	sensor = new Sensor(DHT_PIN, MOISTURE_SENSOR_PIN);
	motor = new Motor(MOTOR_PIN);

	new WebServerEx(WEBSERVER_PORT, sensor, motor);
	Network::mDNSInit(HOSTNAME);
}

void loop() {
	motor->process();
}