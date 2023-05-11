#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>

class Sensor {
   private:
	DHT* dht;
	uint8_t moisturePin;

   public:
	Sensor(uint8_t dhtPin, uint8_t moisturePin);
	~Sensor();
	float readTemperature();
	float readHumidity();
	float readMoisture();
	void read(char* data);
};

#endif