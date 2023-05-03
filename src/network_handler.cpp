#include "network_handler.h"

void wifiClientInit(const char* ssid, const char* password) {
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
}

void wifiAPInit(const char* ssid) {
	WiFi.softAP(ssid);
	Serial.print("AP IP address: ");
	Serial.println(WiFi.softAPIP());
}

void mDNSInit(const char* hostname) {
	if (MDNS.begin(hostname)) {
		Serial.println("MDNS responder started.");
	}
}