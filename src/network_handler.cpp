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

int wifiScan(char* availableNetworks) {
	Serial.println("WiFi scan is started.");
	int n = WiFi.scanNetworks(false, true);
	Serial.println("WiFi scan is completed.");

	DynamicJsonDocument doc(2048);
	JsonArray networks = doc.to<JsonArray>();

	if (n == 0) {
		Serial.println("No networks found.");
	} else {
		Serial.print(n);
		Serial.println(" networks found:");
		for (int i = 0; i < n; ++i) {
			wifi_auth_mode_t security = WiFi.encryptionType(i);

			JsonObject network = networks.createNestedObject();
			network["ssid"] = WiFi.SSID(i);
			network["rssi"] = WiFi.RSSI(i);
			network["bssid"] = WiFi.BSSIDstr(i);

			if (network["ssid"].as<String>().length() == 0) {
				network["ssid"] = "<HIDDEN>";
			}

			if (security == WIFI_AUTH_OPEN) {
				network["security"] = "OPEN";
			} else if (security == WIFI_AUTH_WEP) {
				network["security"] = "WEP";
			} else if (security == WIFI_AUTH_WPA_PSK) {
				network["security"] = "WPA_PSK";
			} else if (security == WIFI_AUTH_WPA2_PSK) {
				network["security"] = "WPA2_PSK";
			} else if (security == WIFI_AUTH_WPA_WPA2_PSK) {
				network["security"] = "WPA_WPA2_PSK";
			} else if (security == WIFI_AUTH_WPA2_ENTERPRISE) {
				network["security"] = "WPA2_ENTERPRISE";
			} else if (security == WIFI_AUTH_WPA3_PSK) {
				network["security"] = "WPA3_PSK";
			} else if (security == WIFI_AUTH_WPA2_WPA3_PSK) {
				network["security"] = "WPA2_WPA3_PSK";
			} else if (security == WIFI_AUTH_WAPI_PSK) {
				network["security"] = "WAPI_PSK";
			} else if (security == WIFI_AUTH_MAX) {
				network["security"] = "MAX";
			}

			Serial.print(i + 1);
			Serial.print(": ");
			Serial.print(network["ssid"].as<const char*>());
			Serial.print(" [");
			Serial.print(network["bssid"].as<const char*>());
			Serial.print("] ");
			Serial.print(" (");
			Serial.print(network["rssi"].as<int>());
			Serial.print(") ");
			Serial.println(network["security"].as<const char*>());
			delay(10);
		}
	}

	String output;
	serializeJson(doc, output);
	strcpy(availableNetworks, output.c_str());

	return n;
}

void mDNSInit(const char* hostname) {
	if (MDNS.begin(hostname)) {
		Serial.println("MDNS responder started.");
	}
}