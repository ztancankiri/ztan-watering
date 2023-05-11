#include "Motor.h"

Motor::Motor(uint8_t motorPin) {
	this->motorPin = motorPin;
	this->isRunning = false;
	this->endTime = 0;
	pinMode(this->motorPin, OUTPUT);
}

Motor::~Motor() {
	this->endTime = 0;
	digitalWrite(this->motorPin, LOW);
	this->isRunning = false;
}

void Motor::run(uint8_t seconds) {
	this->endTime = millis() + seconds * 1000;
	digitalWrite(this->motorPin, HIGH);
	this->isRunning = true;
}

void Motor::process() {
	if (this->isRunning && millis() >= endTime) {
		this->isRunning = false;
		this->endTime = 0;
		digitalWrite(this->motorPin, LOW);
	}
}