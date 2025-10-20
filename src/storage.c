#include "storage.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define PATH_SEPARATOR "\\"
    #define HOME_ENV "USERPROFILE"
#else
    #include <unistd.h>
    #define PATH_SEPARATOR "/"
    #define HOME_ENV "HOME"
#endif

static char* getDataFilePath() {
    static char filePath[512] = {0};
    
    if (filePath[0] != 0) {
        return filePath;  // Already computed
    }
    
    const char* home = getenv(HOME_ENV);
    if (!home) {
        fprintf(stderr, "Error: Could not determine home directory\n");
        exit(1);
    }
    
    snprintf(filePath, sizeof(filePath), "%s%s.todo-cli-data.json", home, PATH_SEPARATOR);
    return filePath;
}

void initStorage() {
    const char* filePath = getDataFilePath();
    
    FILE* file = fopen(filePath, "r");
    if (file) {
        fclose(file);
        return;  // File already exists
    }
    
    // Create default data structure
    cJSON* data = cJSON_CreateObject();
    cJSON* lists = cJSON_CreateObject();
    cJSON* defaultList = cJSON_CreateArray();
    
    cJSON_AddItemToObject(lists, "default", defaultList);
    cJSON_AddItemToObject(data, "lists", lists);
    
    writeData(data);
    cJSON_Delete(data);
}

cJSON* readData() {
    const char* filePath = getDataFilePath();
    
    FILE* file = fopen(filePath, "r");
    if (!file) {
        // Return default structure if file doesn't exist
        cJSON* data = cJSON_CreateObject();
        cJSON* lists = cJSON_CreateObject();
        cJSON* defaultList = cJSON_CreateArray();
        cJSON_AddItemToObject(lists, "default", defaultList);
        cJSON_AddItemToObject(data, "lists", lists);
        return data;
    }
    
    // Read entire file
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    char* content = (char*)malloc(fileSize + 1);
    if (!content) {
        fclose(file);
        fprintf(stderr, "Error: Memory allocation failed\n");
        exit(1);
    }
    
    size_t readSize = fread(content, 1, fileSize, file);
    content[readSize] = '\0';
    fclose(file);
    
    // Parse JSON
    cJSON* data = cJSON_Parse(content);
    free(content);
    
    if (!data) {
        // If JSON is corrupted, return default structure
        cJSON* defaultData = cJSON_CreateObject();
        cJSON* lists = cJSON_CreateObject();
        cJSON* defaultList = cJSON_CreateArray();
        cJSON_AddItemToObject(lists, "default", defaultList);
        cJSON_AddItemToObject(defaultData, "lists", lists);
        return defaultData;
    }
    
    return data;
}

void writeData(cJSON* data) {
    const char* filePath = getDataFilePath();
    
    char* jsonString = cJSON_Print(data);
    if (!jsonString) {
        fprintf(stderr, "Error: Failed to serialize JSON\n");
        return;
    }
    
    FILE* file = fopen(filePath, "w");
    if (!file) {
        fprintf(stderr, "Error: Failed to open file for writing: %s\n", filePath);
        free(jsonString);
        return;
    }
    
    fputs(jsonString, file);
    fclose(file);
    free(jsonString);
}

void freeStorageData(cJSON* data) {
    if (data) {
        cJSON_Delete(data);
    }
}
