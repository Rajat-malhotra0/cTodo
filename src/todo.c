#include "todo.h"
#include "storage.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cJSON.h"
#include <time.h>

static cJSON* globalData = NULL;
static char currentList[256] = "default";

void loadData() {
    if (globalData) {
        cJSON_Delete(globalData);
    }
    globalData = readData();
}

char** getLists(int* count) {
    if (!globalData) {
        *count = 0;
        return NULL;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists || !cJSON_IsObject(lists)) {
        *count = 0;
        return NULL;
    }
    
    // Count lists
    int listCount = 0;
    cJSON* item = NULL;
    cJSON_ArrayForEach(item, lists) {
        listCount++;
    }
    
    // Allocate and copy list names
    char** result = (char**)malloc(sizeof(char*) * listCount);
    if (!result) {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    int i = 0;
    cJSON_ArrayForEach(item, lists) {
        if (item->string) {
            result[i] = (char*)malloc(strlen(item->string) + 1);
            strcpy(result[i], item->string);
            i++;
        }
    }
    
    *count = listCount;
    return result;
}

void freeLists(char** lists, int count) {
    for (int i = 0; i < count; i++) {
        free(lists[i]);
    }
    free(lists);
}

int setCurrentList(const char* listName) {
    if (!globalData) {
        return 0;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists || !cJSON_GetObjectItem(lists, listName)) {
        return 0;
    }
    
    strncpy(currentList, listName, sizeof(currentList) - 1);
    currentList[sizeof(currentList) - 1] = '\0';
    return 1;
}

int createList(const char* listName) {
    if (!globalData || !listName || strlen(listName) == 0) {
        return 0;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists) {
        return 0;
    }
    
    // Check if list already exists
    if (cJSON_GetObjectItem(lists, listName)) {
        return 0;  // Already exists
    }
    
    cJSON* newList = cJSON_CreateArray();
    cJSON_AddItemToObject(lists, listName, newList);
    writeData(globalData);
    
    return 1;
}

Todo* addTodo(const char* task) {
    if (!globalData || !task || strlen(task) == 0) {
        return NULL;
    }
    
    // Trim whitespace
    const char* start = task;
    while (*start && (*start == ' ' || *start == '\t')) start++;
    
    const char* end = start + strlen(start) - 1;
    while (end > start && (*end == ' ' || *end == '\t')) end--;
    
    size_t taskLen = end - start + 1;
    if (taskLen == 0) {
        return NULL;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists) {
        return NULL;
    }
    
    cJSON* currentListArray = cJSON_GetObjectItem(lists, currentList);
    if (!currentListArray || !cJSON_IsArray(currentListArray)) {
        return NULL;
    }
    
    // Create new todo
    cJSON* todoObj = cJSON_CreateObject();
    
    long long id = (long long)time(NULL) * 1000;
    cJSON_AddNumberToObject(todoObj, "id", (double)id);
    
    char trimmedTask[512];
    snprintf(trimmedTask, sizeof(trimmedTask), "%.*s", (int)taskLen, start);
    cJSON_AddStringToObject(todoObj, "task", trimmedTask);
    
    cJSON_AddBoolToObject(todoObj, "completed", 0);
    
    // Add ISO timestamp
    time_t now = time(NULL);
    struct tm* timeinfo = gmtime(&now);
    char timestamp[30];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%dT%H:%M:%SZ", timeinfo);
    cJSON_AddStringToObject(todoObj, "createdAt", timestamp);
    
    cJSON_AddItemToArray(currentListArray, todoObj);
    writeData(globalData);
    
    // Create Todo struct for return
    Todo* result = (Todo*)malloc(sizeof(Todo));
    result->id = id;
    result->task = (char*)malloc(strlen(trimmedTask) + 1);
    strcpy(result->task, trimmedTask);
    result->completed = 0;
    result->createdAt = (char*)malloc(strlen(timestamp) + 1);
    strcpy(result->createdAt, timestamp);
    
    return result;
}

TodoList* listTodos(int showCompleted) {
    if (!globalData) {
        return NULL;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists) {
        return NULL;
    }
    
    cJSON* currentListArray = cJSON_GetObjectItem(lists, currentList);
    if (!currentListArray || !cJSON_IsArray(currentListArray)) {
        return NULL;
    }
    
    TodoList* result = (TodoList*)malloc(sizeof(TodoList));
    result->capacity = 10;
    result->items = (Todo*)malloc(sizeof(Todo) * result->capacity);
    result->count = 0;
    
    cJSON* item = NULL;
    cJSON_ArrayForEach(item, currentListArray) {
        int completed = cJSON_GetObjectItem(item, "completed") &&
                       cJSON_GetObjectItem(item, "completed")->type == cJSON_True;
        
        // Filter based on completion status (-1 means show all)
        if (showCompleted != -1 && (completed != showCompleted)) {
            continue;
        }
        
        // Resize if needed
        if (result->count >= result->capacity) {
            result->capacity *= 2;
            result->items = (Todo*)realloc(result->items, sizeof(Todo) * result->capacity);
        }
        
        Todo* todo = &result->items[result->count];
        
        cJSON* idObj = cJSON_GetObjectItem(item, "id");
        todo->id = idObj ? (long long)idObj->valuedouble : 0;
        
        cJSON* taskObj = cJSON_GetObjectItem(item, "task");
        todo->task = taskObj && taskObj->valuestring ?
                    (char*)malloc(strlen(taskObj->valuestring) + 1) : NULL;
        if (todo->task) {
            strcpy(todo->task, taskObj->valuestring);
        }
        
        todo->completed = completed;
        
        cJSON* createdObj = cJSON_GetObjectItem(item, "createdAt");
        todo->createdAt = createdObj && createdObj->valuestring ?
                         (char*)malloc(strlen(createdObj->valuestring) + 1) : NULL;
        if (todo->createdAt) {
            strcpy(todo->createdAt, createdObj->valuestring);
        }
        
        result->count++;
    }
    
    return result;
}

void freeTodoList(TodoList* list) {
    if (!list) {
        return;
    }
    
    for (int i = 0; i < list->count; i++) {
        if (list->items[i].task) {
            free(list->items[i].task);
        }
        if (list->items[i].createdAt) {
            free(list->items[i].createdAt);
        }
    }
    
    free(list->items);
    free(list);
}

int completeTodo(int index) {
    if (!globalData || index < 0) {
        return 0;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists) {
        return 0;
    }
    
    cJSON* currentListArray = cJSON_GetObjectItem(lists, currentList);
    if (!currentListArray || !cJSON_IsArray(currentListArray)) {
        return 0;
    }
    
    if (index >= cJSON_GetArraySize(currentListArray)) {
        return 0;
    }
    
    cJSON* todoObj = cJSON_GetArrayItem(currentListArray, index);
    if (!todoObj) {
        return 0;
    }
    
    cJSON* completed = cJSON_GetObjectItem(todoObj, "completed");
    if (completed && cJSON_IsBool(completed)) {
        completed->type = (completed->type == cJSON_True) ? cJSON_False : cJSON_True;
    }
    
    writeData(globalData);
    return 1;
}

int deleteTodo(int index) {
    if (!globalData || index < 0) {
        return 0;
    }
    
    cJSON* lists = cJSON_GetObjectItem(globalData, "lists");
    if (!lists) {
        return 0;
    }
    
    cJSON* currentListArray = cJSON_GetObjectItem(lists, currentList);
    if (!currentListArray || !cJSON_IsArray(currentListArray)) {
        return 0;
    }
    
    if (index >= cJSON_GetArraySize(currentListArray)) {
        return 0;
    }
    
    cJSON_DeleteItemFromArray(currentListArray, index);
    writeData(globalData);
    
    return 1;
}

void saveData() {
    if (globalData) {
        writeData(globalData);
    }
}

void cleanupTodoData() {
    if (globalData) {
        cJSON_Delete(globalData);
        globalData = NULL;
    }
}
