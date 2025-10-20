#ifndef STORAGE_H
#define STORAGE_H

#include "cJSON.h"

// Initialize storage and create default file if needed
void initStorage();

// Read data from file and return as JSON
cJSON* readData();

// Write JSON data to file
void writeData(cJSON* data);

// Cleanup allocated memory
void freeStorageData(cJSON* data);

#endif // STORAGE_H
