#include "sensor_handler.h"

Sensor::Sensor(uint8_t dhtPin, uint8_t moisturePin) {
	this->dht = new DHT(dhtPin, DHT22);
	this->moisturePin = moisturePin;
	dht->begin();
	delay(2000);
}

Sensor::~Sensor() {
	delete dht;
}

float Sensor::readTemperature() {
	return dht->readTemperature();
}

float Sensor::readHumidity() {
	return dht->readHumidity();
}

float Sensor::readMoisture() {
	return analogRead(moisturePin);
}

void Sensor::read(char* data) {
	DynamicJsonDocument doc(128);

	doc["temperature"] = readTemperature();
	doc["humidity"] = readHumidity();
	doc["moisture"] = readMoisture();

	String serialized;
	serializeJson(doc, serialized);
	strcpy(data, serialized.c_str());
}