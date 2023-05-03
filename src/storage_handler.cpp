#include "storage_handler.h"

void storageInit() {
	if (!SPIFFS.begin(FORMAT_SPIFFS_IF_FAILED)) {
		Serial.println("SPIFFS Mount Failed. SPIFFS is formatted...");
		ESP.restart();
	}
}

size_t readFile(const char *path, char *buffer) {
	Serial.printf("Reading file: %s\r\n", path);

	File file = SPIFFS.open(path);
	if (!file) {
		Serial.println("- failed to open file for reading");
		return 0;
	}

	size_t s = file.readBytes(buffer, file.size());
	file.close();
	return s;
}

bool writeFile(const char *path, const char *message) {
	Serial.printf("Writing file: %s\r\n", path);

	File file = SPIFFS.open(path, FILE_WRITE);
	if (!file) {
		Serial.println("- failed to open file for writing");
		return false;
	}

	size_t written = file.print(message);
	file.close();

	if (written > 0) {
		Serial.println("- file written");
		return true;
	} else {
		Serial.println("- write failed");
		return false;
	}
}

bool deleteFile(const char *path) {
	Serial.printf("Deleting file: %s\r\n", path);

	if (SPIFFS.remove(path)) {
		Serial.println("- file deleted");
		return true;
	} else {
		Serial.println("- delete failed");
		return false;
	}
}