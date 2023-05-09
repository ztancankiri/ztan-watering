#ifndef ENDPOINT_HANDLER_H
#define ENDPOINT_HANDLER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>

#include "Sensor.h"
#include "network_handler.h"
#include "storage_handler.h"
#include "utils.h"

class WebServerEx {
   private:
	WebServer* server;
	Sensor* sensor;
	TaskHandle_t taskHandle;
	void webServerConfigureEndpoints();
	static void loop(void* instance);

   public:
	WebServerEx(uint8_t port, Sensor* sensor);
	~WebServerEx();
	void webServerProcess();
};

#endif