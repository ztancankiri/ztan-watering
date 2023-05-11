#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
   private:
	uint8_t motorPin;
	uint8_t endTime;
	bool isRunning;

   public:
	Motor(uint8_t motorPin);
	~Motor();
	void run(uint8_t seconds);
	void process();
};

#endif