#ifndef UTILS_HANDLER_H
#define UTILS_HANDLER_H

#include <Arduino.h>

#define CONFIG_FILE "/config.txt"
#define AP_SSID "Ztan-Watering"

bool parseConfig(char *config, const char *delimiter, char *ssid, char *password);

#endif