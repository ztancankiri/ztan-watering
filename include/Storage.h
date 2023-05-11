#ifndef STORAGE_HANDLER_H
#define STORAGE_HANDLER_H

#include <Arduino.h>

#include "FS.h"
#include "SPIFFS.h"

class Storage {
   public:
	static void mount(bool formatOnFail = true);
	static size_t read(const char *path, char *buffer);
	static bool write(const char *path, const char *message);
	static bool remove(const char *path);
};

#endif