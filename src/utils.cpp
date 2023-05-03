#include "utils.h"

bool parseConfig(char *config, const char *delimiter, char *ssid, char *password) {
	char *token = strtok(config, delimiter);
	if (token != NULL) {
		strcpy(ssid, token);
		token = strtok(NULL, delimiter);
		if (token != NULL) {
			strcpy(password, token);
			return true;
		}
	}

	return false;
}