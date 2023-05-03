#ifndef STORAGE_HANDLER_H
#define STORAGE_HANDLER_H

#include <Arduino.h>

#include "FS.h"
#include "SPIFFS.h"

#define FORMAT_SPIFFS_IF_FAILED true

void storageInit();
size_t readFile(const char *path, char *buffer);
bool writeFile(const char *path, const char *message);
bool deleteFile(const char *path);

#endif