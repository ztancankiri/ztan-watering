#include <Arduino.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>

#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true
#define CONFIG_FILE "/config.txt"
#define AP_SSID "Ztan-Watering"

WebServer server(80);

char configBuffer[128] = {'\0'};
size_t configBufferSize = 0;
char ssid[128] = {'\0'};
char password[128] = {'\0'};

size_t readFile(const char *path, char *buffer) {
	Serial.printf("Reading file: %s\r\n", path);

	File file = SPIFFS.open(path);
	if (!file) {
		Serial.println("- failed to open file for reading");
		return 0;
	}

	size_t s = file.readBytes(buffer, file.size());
	file.close();
	return s;
}

bool writeFile(const char *path, const char *message) {
	Serial.printf("Writing file: %s\r\n", path);

	File file = SPIFFS.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("- failed to open file for writing");
		return false;
	}

	size_t written = file.print(message);
	file.close();

	if (written > 0) {
		Serial.println("- file written");
		return true;
	} else {
		Serial.println("- write failed");
		return false;
	}
}

bool deleteFile(const char *path) {
	Serial.printf("Deleting file: %s\r\n", path);

	if (SPIFFS.remove(path)) {
		Serial.println("- file deleted");
		return true;
	} else {
		Serial.println("- delete failed");
		return false;
	}
}

bool parseConfig(char *ssid, char *password) {
	char *token = strtok(configBuffer, "|");
	if (token != NULL) {
		strncpy(ssid, token, configBufferSize - 1);
		token = strtok(NULL, "|");
		if (token != NULL) {
			strncpy(password, token, configBufferSize - 1);
			return true;
		}
	}

	return false;
}

void handleRoot() {
	server.send(200, "text/plain", "Hello from Ztan-Watering!");
}

void handleConfigure() {
	String body = server.arg("plain");

	if (writeFile(CONFIG_FILE, body.c_str())) {
		Serial.println("Config is received: " + body);

		Serial.println("Config is written.");
		server.send(200, "text/plain", "Config is written.");
		delay(1000);
		ESP.restart();
	} else {
		Serial.println("Config is NOT written.");
	}
}

void handleDelete() {
	if (deleteFile(CONFIG_FILE)) {
		Serial.println("Config is deleted.");
		server.send(200, "text/plain", "Config is deleted.");
		delay(1000);
		ESP.restart();
	} else {
		Serial.println("Config is NOT deleted.");
	}
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += (server.method() == HTTP_GET) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for (uint8_t i = 0; i < server.args(); i++) {
		message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
	}

	server.send(404, "text/plain", message);
}

void setup() {
	Serial.begin(9600);

	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println("SPIFFS Mount Failed. SPIFFS is formatted...");
		ESP.restart();
	}

	configBufferSize = readFile(CONFIG_FILE, configBuffer);
	if (configBufferSize > 0) {
		if (!parseConfig(ssid, password)) {
			Serial.println("Could not parse config!");
			if (deleteFile(CONFIG_FILE)) {
				Serial.println("Config is deleted.");
				delay(1000);
				ESP.restart();
			} else {
				Serial.println("Config is NOT deleted.");
				delay(1000);
				ESP.restart();
			}
		}

		WiFi.mode(WIFI_STA);
		WiFi.begin(ssid, password);

		while (WiFi.status() != WL_CONNECTED) {
			delay(500);
			Serial.print(".");
		}

		Serial.println();
		Serial.println("Connected to Wi-Fi.");

		Serial.print("IP address: ");
		Serial.println(WiFi.localIP());
	} else {
		Serial.println("Config Not Exist!");

		WiFi.softAP(AP_SSID);
		Serial.print("AP IP address: ");
		Serial.println(WiFi.softAPIP());
	}

	if (MDNS.begin("ztan-watering")) {
		Serial.println("MDNS responder started.");
	}

	server.on("/", HTTP_GET, handleRoot);
	server.on("/configure", HTTP_POST, handleConfigure);
	server.on("/delete", HTTP_DELETE, handleDelete);
	server.onNotFound(handleNotFound);

	server.begin();
	Serial.println("HTTP server started.");
}

void loop() {
	server.handleClient();
	delay(2);
}