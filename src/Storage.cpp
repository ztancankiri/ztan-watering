#include "Storage.h"

void Storage::mount(bool formatOnFail) {
	if (!SPIFFS.begin(formatOnFail)) {
		Serial.println("SPIFFS Mount Failed. SPIFFS is formatted...");
		ESP.restart();
	}
}

size_t Storage::read(const char *path, char *buffer) {
	File file = SPIFFS.open(path);
	if (!file) {
		return 0;
	}

	size_t fileSize = file.readBytes(buffer, file.size());
	file.close();
	return fileSize;
}

bool Storage::write(const char *path, const char *buffer) {
	File file = SPIFFS.open(path, FILE_WRITE);
	if (!file) {
		return false;
	}

	size_t written = file.print(buffer);
	file.close();

	return written > 0;
}

bool Storage::remove(const char *path) {
	return SPIFFS.remove(path);
}