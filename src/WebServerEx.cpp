#include "WebServerEx.h"

WebServerEx::WebServerEx(uint8_t port, Sensor* sensor, Motor* motor) {
	this->sensor = sensor;
	this->motor = motor;
	server = new WebServer(port);
	webServerConfigureEndpoints();
	server->begin();
	xTaskCreatePinnedToCore(loop, "WebServerExLoop", 10000, this, 0, &taskHandle, 0);
	Serial.println("HTTP server started.");
}

WebServerEx::~WebServerEx() {
	vTaskDelete(taskHandle);
	delete server;
}

void WebServerEx::webServerConfigureEndpoints() {
	server->onNotFound([this]() {
		server->send(404, "text/plain", "Not found!");
	});

	server->on("/", HTTP_GET, [this]() {
		server->send(200, "text/plain", "Hello from Ztan-Watering!");
	});

	server->on("/configure", HTTP_POST, [this]() {
		const char* body = server->arg("plain").c_str();

		StaticJsonDocument<2048> doc;
		DeserializationError error = deserializeJson(doc, body);

		if (error) {
			Serial.println("Config parse error.");
			server->send(400, "text/plain", "Config parse error.");
		} else {
			Configuration::setWiFiSSID(doc["ssid"].as<const char*>());
			Configuration::setWiFiPassword(doc["password"].as<const char*>());
			bool isSaved = Configuration::save();
			doc.clear();

			if (isSaved) {
				server->send(200, "text/plain", "Config is written.");
				delay(1000);
				ESP.restart();
			} else {
				Serial.println("Config is NOT written.");
			}
		}
	});

	server->on("/delete", HTTP_DELETE, [this]() {
		bool isRemoved = Configuration::remove();

		if (isRemoved) {
			Serial.println("Config is deleted.");
			server->send(200, "text/plain", "Config is deleted.");
			delay(1000);
			ESP.restart();
		} else {
			Serial.println("Config is NOT deleted.");
		}
	});

	server->on("/scan", HTTP_GET, [this]() {
		char availableNetworks[2028] = {'\0'};
		int networkCount = Network::scan(availableNetworks);

		if (networkCount > 0) {
			server->send(200, "text/plain", availableNetworks);
		} else {
			server->send(200, "text/plain", "No Network!");
		}
	});

	server->on("/sensor", HTTP_GET, [this]() {
		char sensorData[128] = {'\0'};
		sensor->read(sensorData);

		Serial.print("Sensor Data: ");
		Serial.println(sensorData);

		server->send(200, "text/plain", sensorData);
	});

	server->on("/motor", HTTP_POST, [this]() {
		const char* body = server->arg("plain").c_str();

		StaticJsonDocument<128> doc;
		DeserializationError error = deserializeJson(doc, body);

		if (error) {
			Serial.println("Motor input error.");
			server->send(400, "text/plain", "Motor input error.");
		} else {
			uint8_t duration = doc["duration"];
			motor->run(duration);
			doc.clear();
			server->send(200, "text/plain", "Motor started to run.");
		}
	});
}

void WebServerEx::loop(void* instance) {
	while (true) {
		static_cast<WebServerEx*>(instance)->server->handleClient();
		delay(2);
	}
}