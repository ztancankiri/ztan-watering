#ifndef WEBSERVEREX_H
#define WEBSERVEREX_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <WebServer.h>

#include "Configuration.h"
#include "Motor.h"
#include "Network.h"
#include "Sensor.h"
#include "Storage.h"

class WebServerEx {
   private:
	WebServer* server;
	Sensor* sensor;
	Motor* motor;
	TaskHandle_t taskHandle;
	void webServerConfigureEndpoints();
	static void loop(void* instance);

   public:
	WebServerEx(uint8_t port, Sensor* sensor, Motor* motor);
	~WebServerEx();
	void webServerProcess();
};

#endif