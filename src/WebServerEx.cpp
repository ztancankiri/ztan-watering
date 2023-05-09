#include "WebServerEx.h"

WebServerEx::WebServerEx(uint8_t port, Sensor* sensor) {
	this->sensor = sensor;
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
	server->on("/", HTTP_GET, [this]() {
		server->send(200, "text/plain", "Hello from Ztan-Watering!");
	});

	server->on("/configure", HTTP_POST, [this]() {
		const char* body = server->arg("plain").c_str();

		DynamicJsonDocument doc(128);
		DeserializationError error = deserializeJson(doc, body);

		if (error) {
			Serial.println("Config parse error.");
			server->send(400, "text/plain", "Config parse error.");
		} else {
			char ssid[128], password[128] = {'\0'};

			DynamicJsonDocument newDoc(128);
			newDoc["ssid"] = doc["ssid"];
			newDoc["password"] = doc["password"];

			String output;
			serializeJson(newDoc, output);

			if (writeFile(CONFIG_FILE, output.c_str())) {
				Serial.print("Config is received: ");
				Serial.println(body);
				Serial.println("Config is written.");

				server->send(200, "text/plain", "Config is written.");
			} else {
				Serial.println("Config is NOT written.");
			}

			delay(1000);
			ESP.restart();
		}
	});

	server->on("/delete", HTTP_DELETE, [this]() {
		if (deleteFile(CONFIG_FILE)) {
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
		int networkCount = wifiScan(availableNetworks);

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

	server->onNotFound([this]() {
		String message = "File Not Found\n\n";
		message += "URI: ";
		message += server->uri();
		message += "\nMethod: ";
		message += (server->method() == HTTP_GET) ? "GET" : "POST";
		message += "\nArguments: ";
		message += server->args();
		message += "\n";

		for (uint8_t i = 0; i < server->args(); i++) {
			message += " " + server->argName(i) + ": " + server->arg(i) + "\n";
		}

		server->send(404, "text/plain", message);
	});
}

void WebServerEx::loop(void* instance) {
	while (true) {
		static_cast<WebServerEx*>(instance)->server->handleClient();
		delay(2);
	}
}