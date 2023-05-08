#include "webserver_handler.h"

void webServerInit(WebServer* server) {
	server->on("/", HTTP_GET, [server]() {
		server->send(200, "text/plain", "Hello from Ztan-Watering!");
	});

	server->on("/configure", HTTP_POST, [server]() {
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

	server->on("/delete", HTTP_DELETE, [server]() {
		if (deleteFile(CONFIG_FILE)) {
			Serial.println("Config is deleted.");
			server->send(200, "text/plain", "Config is deleted.");
			delay(1000);
			ESP.restart();
		} else {
			Serial.println("Config is NOT deleted.");
		}
	});

	server->onNotFound([server]() {
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

	server->begin();
	Serial.println("HTTP server started.");
}

void webServerProcess(WebServer* server) {
	server->handleClient();
	delay(2);
}